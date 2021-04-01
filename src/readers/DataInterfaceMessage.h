/* Copyright 2020 The Loimos Project Developers.
 * See the top-level LICENSE file for details.
 *
 * SPDX-License-Identifier: MIT
 */

#include "../loimos.decl.h"

#ifndef __DATA_INTERFACE_MESSAGE_H__
#define __DATA_INTERFACE_MESSAGE_H__

class DataInterfaceMessage : public CMessage_DataInterfaceMessage {
    public:
        int numDataAttributes;
        Data *dataAttributes;
        int uniqueId;

        DataInterfaceMessage(int attributes) {
            numDataAttributes = attributes;
            if (numDataAttributes != 0) {
                dataAttributes = new Data[numDataAttributes];
            } else {
                dataAttributes = NULL;
            }
        }
        
        /** Need to manually pack/unpack dynamic size message */
        static void *pack(DataInterfaceMessage* msg) {
            // Determine total size of unpacked message in buffer.
            int numDataAttributes = msg->numDataAttributes;
            size_t dataAttributeSize = sizeof(Data) * numDataAttributes; 
            size_t messageSize = dataAttributeSize + 2 * sizeof(int);
            // Allocate space and associate that space with the message.
            int *data = (int *) CkAllocBuffer(msg, messageSize);
            // Copy attributes to the new bufffer.
            data[0] = msg->uniqueId;
            data[1] = msg->numDataAttributes;
            if (numDataAttributes != 0) {
                memcpy(&data[2], msg->dataAttributes, dataAttributeSize);
            }
            // Free message.
            delete[] msg->dataAttributes;
            CkFreeMsg(msg);
            return data;
        }

        static void *unpack(void *buf) {
            // Create space for the new message.
            DataInterfaceMessage *msg = (DataInterfaceMessage *) CkAllocBuffer(buf, sizeof(DataInterfaceMessage));
            // Unpack array.
            int *data = (int *) buf;
            // Copy fixed int fields into message.
            msg->uniqueId = data[0];
            msg->numDataAttributes = data[1];
            // Copy data fields to message.
            if (msg->numDataAttributes != 0) {
                msg->dataAttributes = new Data[msg->numDataAttributes];
                memcpy(msg->dataAttributes, &data[2], sizeof(Data) * msg->numDataAttributes);
            } else {
                msg->dataAttributes = NULL;
            }
            CkFreeMsg(buf);
            return msg;
        }
};

#endif // __DATA_INTERFACE_MESSAGE_H__