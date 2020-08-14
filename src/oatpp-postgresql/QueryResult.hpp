/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_postgresql_QueryResult_hpp
#define oatpp_postgresql_QueryResult_hpp

#include "ConnectionProvider.hpp"
#include "mapping/Deserializer.hpp"
#include "mapping/ResultMapper.hpp"
#include "oatpp/orm/QueryResult.hpp"

namespace oatpp { namespace postgresql {

class QueryResult : public orm::QueryResult {
private:
  static constexpr v_int32 TYPE_ERROR = 0;
  static constexpr v_int32 TYPE_COMMAND = 1;
  static constexpr v_int32 TYPE_TUPLES = 2;
private:
  PGresult* m_dbResult;
  std::shared_ptr<Connection> m_connection;
  std::shared_ptr<mapping::ResultMapper> m_resultMapper;
  mapping::ResultMapper::ResultData m_resultData;
  bool m_success;
  v_int32 m_type;
private:
  mapping::Deserializer m_deserializer;
public:

  QueryResult(PGresult* dbResult,
              const std::shared_ptr<Connection>& connection,
              const std::shared_ptr<mapping::ResultMapper>& resultMapper);

  ~QueryResult();

  bool isSuccess() const override;

  v_int64 position() const override;

  v_int64 count() const override;

  void fetch(oatpp::Void& polymorph, v_int64 count) override;

};

}}

#endif //oatpp_postgresql_QueryResult_hpp