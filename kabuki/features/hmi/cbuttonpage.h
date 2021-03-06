/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_page_button.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_PAGEBUTTON
#define HEADER_FOR_KT_HMI_PAGEBUTTON

#include "button.h"
#include "control_layer.h"
#include "widget_page.h"

namespace _ {

/* A type of Button that loads a WidgetPage into a Control Layer
    The difference between a page and a macro button is that a page button
    swtiches between pages in a template and a PageButton will be able to
   perform other tasks in a virtual instrument such as loading one of the
   sub-menus. Not sure if this class is neccissary yet. */
class API PageButton : public Button {
 public:
  //< Constructor.
  PageButton(const WidgetPage& thePage = WidgetPage());

  //< Copy constructor.
  PageButton(const PageButton& objToCopy);

  //< Destructor.
  ~PageButton();

  //< The action  (s) performed when this button gets pressed.
  void press(const ControlLayer& parentLayer);

  //< The action  (s) performed when this button gets double pressed.
  void depress(const ControlLayer& parentLayer);

  //< The action  (s) performed when this button gets depressed.
  void depress(const ControlLayer& parentLayer);

  //< Gets th WidgetPage that this
  WidgetPage* GetPage();

  //< Sets thisPage to the newPage.
  void SetPage(WidgetPage* newPage);

  /* Prints this object to a terminal. */
  _::Utf& Print(_::Utf& print) const;

 private:
  WidgetPage* thisPage;  //< The page to load.
};
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_PAGEBUTTON
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
