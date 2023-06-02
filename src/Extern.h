/* Copyright 2020 The Loimos Project Developers.
 * See the top-level LICENSE file for details.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __EXTERN_H__
#define __EXTERN_H__


extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ CProxy_People peopleArray;
extern /* readonly */ CProxy_Locations locationsArray;
#ifdef USE_HYPERCOMM
extern /* readonly */ CProxy_Aggregator aggregatorProxy;
#endif
extern /* readonly */ CProxy_DiseaseModel globDiseaseModel;
extern /* readonly */ int numPeople;
extern /* readonly */ int numLocations;
extern /* readonly */ int numPeoplePartitions;
extern /* readonly */ int numLocationPartitions;
extern /* readonly */ int numDays;
extern /* readonly */ int numDaysWithRealData;
extern /* readonly */ bool syntheticRun;

extern /* readonly */ uint64_t totalVisits;
extern /* readonly */ double simulationStartTime;
extern /* readonly */ double iterationStartTime;

// For real data run.
extern /* readonly */ std::string scenarioPath;
extern /* readonly */ std::string scenarioId;
extern /* readonly */ int firstPersonIdx;
extern /* readonly */ int firstLocationIdx;


// For synthetic run.
extern /* readonly */ int synPeopleGridWidth;
extern /* readonly */ int synPeopleGridHeight;
extern /* readonly */ int synLocationGridWidth;
extern /* readonly */ int synLocationGridHeight;
extern /* readonly */ int synLocalLocationGridWidth;
extern /* readonly */ int synLocalLocationGridHeight;
extern /* readonly */ int synLocationPartitionGridWidth;
extern /* readonly */ int synLocationPartitionGridHeight;
extern /* readonly */ int averageDegreeOfVisit;

// Intervention
extern /* readonly */ bool interventionStategy;

#endif //__EXTERN_H__
