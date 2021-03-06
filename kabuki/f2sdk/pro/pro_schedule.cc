/* Kabuki Toolkit
@version 0.x
@file    /libraries/pro/impl/schedule.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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

#include <pch.h>

#if SEAM >= SEAM_0_0_0__07
#include "schedule.h"

namespace _ {

Schedule::Schedule(const char* key, int num_repeats, color_t color)
    : key_(TextClone(key)),
      tasks_(),
      color_(color),
      num_repeats_(num_repeats) {}

Schedule::~Schedule() {}

const char* Schedule::GetKey() { return key_; }

void Schedule::StealKey(char* new_name) {
  if (new_name == nullptr) return;
  delete key_;
  key_ = new_name;
}

void Schedule::SetKey(const char* new_name) {
  return StealKey(TextClone(new_name));
}

const char* Schedule::GetReadme() { return readme_; }

void Schedule::StealReadMe(char* new_readme) {
  if (new_readme == nullptr) return;
  delete readme_;
  readme_ = new_readme;
}

void Schedule::SetReadMe(const char* new_readme) {
  return StealReadMe(TextClone(new_readme));
}

int Schedule::GetColor() { return color_; }

void Schedule::SetColor(int new_color) { color_ = new_color; }

bool Schedule::Remove(int index) { return tasks_.Remove(index); }

void Schedule::Add(Task* task) { tasks_.Add(task); }

Task* Schedule::Find(Tms event_time) {
  // @todo Replace me with std::map!
  for (int i = 0; i < tasks_.GetCount(); ++i) {
    Task* task = tasks_[i];
    if (task->Contains(event_time)) return task;
  }
  return nullptr;
}

const char* Schedule::GetHelpString() {
  return "\n| Schedule: A (optimally)recurring collection of "
         "chronological Tasks."
         "\n| Schedule Tasks can be addressed by entering the index."
         "\n| The rest of the char is then passed on to the task "
         "\n| at the given index."
         "\n|"
         "\n| Schedule Commands:"
         "\n|   -a    Add a new Task."
         "\n|         Example: -a -string \"Header\" -d \"Details\" -b 3:30"
         "\n|                  -e 4:30 -w 10.0"
         "\n|"
         "\n|   -p    Prints the Schedule."
         "\n|         Example: -p"
         "\n|"
         "\n|   -r    Removes a task by index."
         "\n|         Example: -r 2"
         "\n";
}

void Schedule::Print(int indentation) {
  PrintBreak("\n<", '-');
  std::cout << "< Schedule: " << key_ << " num_elements: " << tasks_.GetCount()
            << "\n";
  PrintBreak("\n<", '-');
  for (int i = 0; i < tasks_.GetCount(); ++i) {
    std::cout << "<  " << i << ": \"" << tasks_[i]->GetHeader() << "\"\n";
  }
  PrintBreak("<", '-');
}

const char* Schedule::Command(char* input) {
  char c,     //< The first char.
      d;      //< The second char.
  int value;  //< An index to possibly be scanned.
  const char* result;
  Task* t;

  input = SkipSpaces(input);
  std::cout << "> " << key_ << " > " << input << '\n';
  c = *input;
  if (!c) return "No Schedule arguments read";
  // std::cout << "> Schedule ";
  if (c == '-') {
    c = *(input + 1);
    d = *(input + 2);
    // std::cout << '-' << c << d << '\n';
    if (d && !isspace(d)) return "Invalid Schedule flag";
    input = SkipSpaces(input);
    switch (c) {
      case 'a':  // Add new Task
      {
        std::cout << "< Adding Task: \"" << input + 3 << "\"\n";
        input = SkipSpaces(input + 2);
        c = *input;
        if (c != '-') return "You must add at least one flag";
        t = new Task();
        if (result = t->Command(input)) return result;
        tasks_.Add(t);
        return nullptr;
      }
      case 'p':  // Print
      {
        // First try to find an integer to print.

        if (!sscanf(input, "%i", &value)) {
          Print();
          return nullptr;
        }
        if (value < 0 || value >= tasks_.GetCount())
          return "Invalid task index";
        tasks_[value]->Print();
        return nullptr;
      }
      case 'r':  //< Remove
      {
        input += 3;
        // input = skip_spaces (input);
        c = *input;
        if (!isdigit(c)) return "Error scanning index to remove";
        if (!sscanf(input, "%i", &value))  //< This shouldn't happen.
          return "Error scanning index to remove";
        std::cout << "| Removing task " << value << '\n';
        if (value > tasks_.GetCount()) return "Invalid schedule index";
        Remove(value);
        return nullptr;
      }
    }
    return "Invalid Schedule flag argument";
  } else if (isdigit(c)) {
    // Because we checked for the -flag before this, we'll never have a
    // negative number!
    if (!sscanf(input, "%i", &value))  // This shouldn't happen.
      return "Error reading Project member index";
    if (value < tasks_.GetCount())
      return tasks_[value]->Command(TextTokenEnd(input) + 1);
    return "Invalid Project member index";
  } else if (c == '\"') {
    return "Search feature not implemented";
  } else if (c == '?')  // Help
  {
    std::cout << GetHelpString();
    return input;
  }

  return "Invalid Schedule command";
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
