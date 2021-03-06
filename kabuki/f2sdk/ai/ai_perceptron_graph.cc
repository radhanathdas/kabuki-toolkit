/** Kabuki Toolkit@version 0.x
@file    /kabuki/f2sdk/ai/aiml_perceptron_layer.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#include "perceptron_graph.h"

namespace _ {

PerceptronGraph::PerceptronGraph (size_t  output_count,
                                  float_t output_bias,
                                  size_t  input_count,
                                  float_t input_bias) :
    x_ (input_count, input_bias),
    y_ (output_count, output_bias),
    layer_1_ () {
    x_.reserve (input_count);
}

std::vector<float_t>& PerceptronGraph::GetX () {
    return x_;
}

float_t* PerceptronGraph::GetX (size_t index) {
    if (index >= x_.size ())
        return nullptr;
    return x_[index];
}

std::vector<float_t>& PerceptronGraph::GetY () {
    return y_;
}

void PerceptronGraph::Update () {
    layer_1_.Update ();
}

}       // namespace _
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1
