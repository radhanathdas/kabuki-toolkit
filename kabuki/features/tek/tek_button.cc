/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/button.cpp
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include "button.h"

using namespace _ {;

namespace kabuki { namespace tek {

Button::Button (offset_t bit)
    : bit_offset_ (bit) {
}

inline byte Button::GetState (byte* digital_ins) {
    if (!digital_ins) {
        return 0;
    }
    offset_t bit = bit_offset_;
    return digital_ins[bit >> kBitToByteShift] & (1 << (bit & kBitNumberMask));
}

inline void Button::Poll (byte* debounced_xor) {
    offset_t index = bit_offset_ >> kBitToByteShift,
             mask  = 1 << (bit_offset_ & kBitNumberMask),
             state = debounced_xor[index] & mask;

    if (state == 0) {
        return;
    }
    if (debounced_xor[index] & mask) {
        press_->Trigger ();
        return;
    }

    depress_->Trigger ();
}

ButtonOp::ButtonOp (Button* object) :
	object_ (object) {
	// Nothing to do here!
}

const Operation* ButtonOp::Star (char_t index, Expr* expr) {
	object_->Star (index, expr);
}

}       //< namespace tek
}       //< namespace kabuki
