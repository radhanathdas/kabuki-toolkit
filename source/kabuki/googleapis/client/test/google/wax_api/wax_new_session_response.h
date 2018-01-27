// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

// This code was generated by google-apis-code-generator 1.5.1

// ----------------------------------------------------------------------------
// NOTE: This file is generated from Google APIs Discovery Service.
// Service:
//   Wax API (wax/v1)
// Generated from:
//   Version: v1
//   Revision: 20130321
// Generated by:
//    Tool: google-apis-code-generator 1.5.1
//     C++: 0.1.3
#ifndef  GOOGLE_WAX_API_WAX_NEW_SESSION_RESPONSE_H_
#define  GOOGLE_WAX_API_WAX_NEW_SESSION_RESPONSE_H_

#include <string>
#include "googleapis/base/macros.h"
#include "googleapis/client/data/jsoncpp_data.h"
#include "googleapis/strings/stringpiece.h"

namespace Json {
class Value;
}  // namespace Json

namespace google_wax_api {
using namespace googleapis;

/**
 * No description provided.
 *
 * @ingroup DataObject
 */
class WaxNewSessionResponse : public client::JsonCppData {
 public:
  /**
   * Creates a new default instance.
   *
   * @return Ownership is passed back to the caller.
   */
  static WaxNewSessionResponse* New();

  /**
   * Standard constructor for an immutable data object instance.
   *
   * @param[in] storage  The underlying data storage for this instance.
   */
  explicit WaxNewSessionResponse(const Json::Value& storage);

  /**
   * Standard constructor for a mutable data object instance.
   *
   * @param[in] storage  The underlying data storage for this instance.
   */
  explicit WaxNewSessionResponse(Json::Value* storage);

  /**
   * Standard destructor.
   */
  virtual ~WaxNewSessionResponse();

  /**
   * Returns a string denoting the type of this data object.
   *
   * @return <code>google_wax_api::WaxNewSessionResponse</code>
   */
  const StringPiece GetTypeName() const {
    return StringPiece("google_wax_api::WaxNewSessionResponse");
  }

  /**
   * Determine if the '<code>kind</code>' attribute was set.
   *
   * @return true if the '<code>kind</code>' attribute was set.
   */
  bool has_kind() const {
    return Storage().isMember("kind");
  }

  /**
   * Clears the '<code>kind</code>' attribute.
   */
  void clear_kind() {
    MutableStorage()->removeMember("kind");
  }


  /**
   * Get the value of the '<code>kind</code>' attribute.
   */
  const StringPiece get_kind() const {
    const Json::Value& v = Storage("kind");
    if (v == Json::Value::null) return StringPiece("");
    return StringPiece(v.asCString());
  }

  /**
   * Change the '<code>kind</code>' attribute.
   *
   * The kind of object this is.
   *
   * @param[in] value The new value.
   */
  void set_kind(const StringPiece& value) {
    *MutableStorage("kind") = value.data();
  }

  /**
   * Determine if the '<code>newSessionId</code>' attribute was set.
   *
   * @return true if the '<code>newSessionId</code>' attribute was set.
   */
  bool has_new_session_id() const {
    return Storage().isMember("newSessionId");
  }

  /**
   * Clears the '<code>newSessionId</code>' attribute.
   */
  void clear_new_session_id() {
    MutableStorage()->removeMember("newSessionId");
  }


  /**
   * Get the value of the '<code>newSessionId</code>' attribute.
   */
  const StringPiece get_new_session_id() const {
    const Json::Value& v = Storage("newSessionId");
    if (v == Json::Value::null) return StringPiece("");
    return StringPiece(v.asCString());
  }

  /**
   * Change the '<code>newSessionId</code>' attribute.
   *
   * The session ID.
   *
   * @param[in] value The new value.
   */
  void set_new_session_id(const StringPiece& value) {
    *MutableStorage("newSessionId") = value.data();
  }

 private:
  void operator=(const WaxNewSessionResponse&);
};  // WaxNewSessionResponse
}  // namespace google_wax_api
#endif  // GOOGLE_WAX_API_WAX_NEW_SESSION_RESPONSE_H_
