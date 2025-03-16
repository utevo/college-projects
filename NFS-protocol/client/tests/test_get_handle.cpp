#include "test_helper.h"

#include "../command_handlers/handlers.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gmock/gmock-more-matchers.h>

#include <gmock/gmock-more-actions.h>
#include <gmock/gmock-generated-actions.h>

using ::testing::_;
using namespace testing;

TEST(get_handler, connection_closes_after_saver_opening_error){

    mock_connector conn;
    NiceMock<mock_saver_> s;
    Command c;
    c.type = Command::Get;
    c.command_string = std::string("get test_file.txt test_file.txt");

    ON_CALL(conn, open(_, _, _)).WillByDefault(Return(1));
    ON_CALL(conn, read(_, _, _)).WillByDefault(Return(0));
    ON_CALL(s, open(_)).WillByDefault(Return(false));
    EXPECT_CALL(conn, open(_, _, _)).Times(0);

    EXPECT_CALL(conn, read(_, _, _))
        .Times(0);
    
    EXPECT_CALL(conn, close(_)).Times(0);

    EXPECT_FALSE(get_handler(c, &s, &conn));

}

TEST(get_handler, connection_closes_after_empty_file){

    mock_connector conn;
    ::testing::NiceMock<mock_saver_> s;
    Command c;
    c.type = Command::Get;
    c.command_string = std::string("get test_file.txt test_file.txt");

    ON_CALL(conn, open(_, _, _)).WillByDefault(Return(1));
    ON_CALL(conn, read(_, _, _)).WillByDefault(Return(0));
    ON_CALL(s, open(_)).WillByDefault(Return(true));
    
    EXPECT_CALL(conn, open(_, _, _)).Times(1);
    EXPECT_CALL(conn, read(_, _, _))
        .Times(1);
    
    EXPECT_CALL(conn, close(_)).Times(1);

    EXPECT_CALL(s, save(_, 0)).Times(0);
    EXPECT_CALL(s, close()).Times(1);

    EXPECT_TRUE(get_handler(c, &s, &conn));

}

TEST(get_handler, everything_fine){

    mock_connector conn;
    ::testing::NiceMock<mock_saver_> s;
    Command c;
    c.type = Command::Get;
    c.command_string = std::string("get test_file.txt test_file.txt");

    std::vector<long int> tab{20, 0};
    int idx = 0;

    ON_CALL(conn, open(_, _, _)).WillByDefault(Return(1));
    
    ON_CALL(conn, read(_, _, _)).WillByDefault(Invoke(
        [tab, &idx](int a, char* b, long unsigned int v){return tab[idx++];}
    ));

    ON_CALL(s, open(_)).WillByDefault(Return(true));
    
    EXPECT_CALL(conn, open(_, _, _)).Times(1);
    EXPECT_CALL(conn, read(_, _, _))
        .Times(2);
    
    EXPECT_CALL(conn, close(_)).Times(1);

    EXPECT_CALL(s, save(_, 20)).Times(1);
    EXPECT_CALL(s, save(_, 0)).Times(0);
    EXPECT_CALL(s, close()).Times(1);

    EXPECT_TRUE(get_handler(c, &s, &conn));

}

int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



