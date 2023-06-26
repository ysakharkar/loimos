 /* Copyright 2020-2023 The Loimos Project Developers.
  * See the top-level LICENSE file for details.
  *
  * SPDX-License-Identifier: MIT
  */

#include "Interventions.h"
#include "AttributeTable.h"
#include "readers/DataInterface.h"
#include "readers/interventions.pb.h"

#include <vector>

bool Intervention::test(const DataInterface &p,
    std::default_random_engine *generator) const {
  return false;
}

void Intervention::apply(DataInterface *p) const {}

void Intervention::pup(PUP::er &p) {}

VaccinationIntervention::VaccinationIntervention(
    const loimos::proto::InterventionModel::Intervention &interventionDef,
    const AttributeTable &t) {
  vaccinationProbability = interventionDef.vaccination().probability();
  vaccinatedSusceptibility = interventionDef.vaccination()
    .vaccinated_susceptibility();
  this->vaccinatedIndex = t.getAttribute("vaccinated");
  this->susceptibilityIndex = t.getAttribute("susceptibility");
}

void VaccinationIntervention::pup(PUP::er &p) {
  p | vaccinationProbability;
  p | vaccinatedIndex;
  p | susceptibilityIndex;
}

bool VaccinationIntervention::test(const DataInterface &p,
    std::default_random_engine *generator) const {
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  // CkPrintf("Vaccinated:%d, riskProbability: %f, selected for vaccine %d\n",
  //     p.getValue(vaccinatedIndex).boolean, p.getValue(riskIndex).probability,
  //     applied);
  return !p.getValue(vaccinatedIndex).boolean
    && distribution(*generator) < vaccinationProbability;
}

void VaccinationIntervention::apply(DataInterface *p) const {
  std::vector<union Data> &objData = p->getData();
  objData[vaccinatedIndex].boolean = true;
  objData[susceptibilityIndex].double_b10 = vaccinatedSusceptibility;
}