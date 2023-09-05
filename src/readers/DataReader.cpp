/* Copyright 2020-2023 The Loimos Project Developers.
 * See the top-level LICENSE file for details.
 *
 * SPDX-License-Identifier: MIT
 */

#include "DataReader.h"

#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <tuple>
#include <fcntl.h>
#include <sys/stat.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

/**
 * If path does not name an existing file, create a directory
 * there, with permissions match that of refPath. Return whether
 * or not a new directory was created
 * Should have behavior analogous to C++17 std::filesystem::create_directory
 */
bool createDirectory(std::string path, std::string refPath) {
  struct stat dirStat;
  if (0 == stat(path.c_str(), &dirStat)) {
    struct stat refStat;
    stat(refPath.c_str(), &refStat);
    return 0 == mkdir(path.c_str(), refStat.st_mode);
  }
  return false;
}

void readProtobuf(std::string path, google::protobuf::Message *buffer) {
  int fd = open(path.c_str(), O_RDONLY);
  if (0 > fd) {
    CkAbort("Error: unable to read %s", path.c_str());
  }
  google::protobuf::io::FileInputStream fstream(fd);
  google::protobuf::TextFormat::Parse(&fstream, buffer);
  close(fd);
}

std::tuple<Id, Id, Time, Time> parseActivityStream(std::ifstream *input,
    loimos::proto::CSVDefinition *dataFormat, std::vector<union Data> *attributes) {
  Id personId = -1;
  Id locationId = -1;
  Time startTime = -1;
  Time duration = -1;
  // TODO(IanCostello) don't reallocate this every time.
  char buf[MAX_INPUT_lineLength];

  // Get header line.
  input->getline(buf, MAX_INPUT_lineLength);

  // Read over people data format.
  int attrIndex = 0;
  int numDataFields = 0;
  int leftCommaLocation = 0;

  int lineLength = input->gcount();
  for (int c = 0; c < lineLength; c++) {
    // Scan for the next attributes - comma separated.
    if (buf[c] == CSV_DELIM || c + 1 == lineLength) {
      // Get next attribute type.
      loimos::proto::DataField const *field = &dataFormat->fields(attrIndex);
      uint16_t dataLen = c - leftCommaLocation;
      if (field->has_ignore() || dataLen == 0) {
        // Skip
      } else if (numDataFields <= 3) {
        // Process data.
        char *start = buf + leftCommaLocation;
        if (c + 1 == lineLength) {
          dataLen += 1;
        } else {
          start[dataLen] = 0;
        }

        // Parse byte stream to the correct representation.
        if (field->has_unique_id()) {
          personId = ID_PARSE(start);

        } else if (field->has_foreign_id()) {
          locationId = ID_PARSE(start);

        } else if (field->has_start_time()) {
          startTime = TIME_PARSE(start);

        } else if (field->has_duration()) {
          duration = TIME_PARSE(start);

        } else {
          numDataFields++;
        }
      }
      leftCommaLocation = c + 1;
      attrIndex++;
    }
  }
  return std::make_tuple(personId, locationId, startTime, duration);
}
