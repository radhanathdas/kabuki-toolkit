/** TypeCraft - Teaching Typing through Mining and Crafting.
@link    https://github.com/kabuki-starship/kabuki-projects/typecraft
@file    ~/typecraft/inventory.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#ifndef TYPECRAFT_INVENTORY_H
#define TYPECRAFT_INVENTORY_H

#include "../../../kabuki-toolkit/source/kabuki/data/include/array.h"
#include "item_group.h"

namespace typecraft {

class Inventory {
 public:
  /** Constructs an Inventory with the given max size. */
  Inventory(int max_size);

  /** Gets inventory count. */
  int GetCount();

  /** Adds an Item. */
  int AddItem(Item* item);

  /** Removes the Item with the given index.
      @param  The index of the Item to remove.
      @return Returns nullptr if the index is out of bounds and a pointer to
              the Item it removed upon success. */
  Item* RemoveItem(int index);

  /** Deletes the entire Inventory. */
  void DeleteAll();

  /** Prints the Inventory to the console. */
  void Print(_::Log& log);

 private:
  ArX<ItemGroup*> items_;  //< An array of Inventory Items.
};

}  // namespace typecraft
#endif  //< TYPECRAFT_INVENTORY_H
