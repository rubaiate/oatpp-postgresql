
#include "oatpp-postgresql/Executor.hpp"

#include "oatpp-test/UnitTest.hpp"

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/base/Environment.hpp"

#include <iostream>


#include "oatpp/database/DbClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace {

#include OATPP_CODEGEN_BEGIN(DbClient)

class MyClient : public oatpp::database::DbClient {
public:

  MyClient(const std::shared_ptr<oatpp::database::Executor>& executor)
    : oatpp::database::DbClient(executor)
  {}

  QUERY(getUserById, "SELECT * FROM user WHERE tag=$<text>$a$<text>$ AND userId=:userId AND role=:role",
        PARAM(oatpp::String, userId),
        PARAM(oatpp::String, role))

  QUERY(createUser,
        "INSERT INTO EXAMPLE_USER "
        "(userId, login, password, email) VALUES "
        "(uuid_generate_v4(), :login, :password, :email) "
        "RETURNING *;",
        PARAM(oatpp::String, login),
        PARAM(oatpp::String, password),
        PARAM(oatpp::String, email))

};

#include OATPP_CODEGEN_END(DbClient)

class Test : public oatpp::test::UnitTest {
public:
  Test() : oatpp::test::UnitTest("MyTag")
  {}

  void onRun() override {

    auto executor = std::make_shared<oatpp::postgresql::Executor>();
    auto client = MyClient(executor);
    auto connection = executor->getConnection();

    client.createUser("my-login", "pass", "email@email.com", connection);

  }
};

void runTests() {
  OATPP_RUN_TEST(Test);
}

}

int main() {

  oatpp::base::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();

  return 0;
}
