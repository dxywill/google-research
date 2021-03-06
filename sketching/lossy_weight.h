// Copyright 2019 The Google Research Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SKETCHING_LOSSY_WEIGHT_H_
#define SKETCHING_LOSSY_WEIGHT_H_

// A hybrid of LossyCount and MisraGries - this can accept any updates (not just
// >= 1). It lazily removes items, so is able to obtain the same accuracy as
// LossyCount with significantly less memory. However it is slower as it needs
// to determine the threshold dynamically, so it is about as slow as MisraGries.

#include <vector>

#include "sketch.h"
#include "countmin.h"

namespace sketch {

class LossyWeight : public Sketch {
 public:
  LossyWeight(uint window_size, uint hash_count, uint hash_size);

  virtual ~LossyWeight() {}

  virtual void Reset();

  virtual void Add(uint item, float delta);

  virtual void ReadyToEstimate() { MergeCounters(); }

  virtual float Estimate(uint item) const;

  virtual void HeavyHitters(float threshold, std::vector<uint>* items) const;

  virtual unsigned int Size() const;

  virtual bool Compatible(const Sketch& other_sketch) const;

  virtual void Merge(const Sketch& other_sketch);

 private:
  uint window_size_;
  uint accumulated_counters_;
  std::vector<IntFloatPair> counters_;
  CountMinCU cm_;

  // Merge duplicate counters in counters_
  void MergeCounters();
};

}  // namespace sketch

#endif  // SKETCHING_LOSSY_WEIGHT_H_
