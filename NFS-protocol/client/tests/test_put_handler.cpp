#include "test_helper.h"

#include "../command_handlers/handlers.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gmock/gmock-more-matchers.h>

#include <gmock/gmock-more-actions.h>
#include <gmock/gmock-generated-actions.h>

using ::testing::_;
using namespace testing;

TEST(put_handler, reader_opening_error){

    mock_connector conn;
    NiceMock<mock_reader_> s;
    Command c;
    c.type = Command::Put;
    c.command_string = std::string("put test_file.txt test_file.txt");

    ON_CALL(conn, open(_, _, _)).WillByDefault(Return(0));
    ON_CALL(conn, read(_, _, _)).WillByDefault(Return(0));
    ON_CALL(s, open(_)).WillByDefault(Return(false));
    EXPECT_CALL(conn, open(_, _, _)).Times(0);

    EXPECT_CALL(conn, read(_, _, _))
        .Times(0);
    
    EXPECT_CALL(conn, close(_)).Times(0);

    EXPECT_FALSE(put_handler(c, &s, &conn));

}

TEST(put_handler, empty_buffer_read){

    mock_connector conn;
    ::testing::NiceMock<mock_reader_> s;
    Command c;
    c.type = Command::Put;
    c.command_string = std::string("get test_file.txt test_file.txt");

    ON_CALL(conn, open(_, _, _)).WillByDefault(Return(1));
    ON_CALL(s, read(_, _)).WillByDefault(Return(0));
    ON_CALL(s, open(_)).WillByDefault(Return(true));
    
    EXPECT_CALL(conn, open(_, _, _)).Times(1);
    EXPECT_CALL(conn, write(_, _, _))
        .Times(0);
    
    EXPECT_CALL(conn, close(_)).Times(1);
    EXPECT_CALL(s, close()).Times(1);

    EXPECT_TRUE(put_handler(c, &s, &conn));

}

TEST(put_handler, everything_fine){

    mock_connector conn;
    ::testing::NiceMock<mock_reader_> s;
    Command c;
    c.type = Command::Put;
    c.command_string = std::string("get test_file.txt test_file.txt");

    std::vector<long int> tab{20, 0};
    int idx = 0;

    ON_CALL(conn, open(_, _, _)).WillByDefault(Return(1));
    
    ON_CALL(s, read(_, _)).WillByDefault(Invoke(
        [tab, &idx](char* a, int){return tab[idx++];}
    ));

    ON_CALL(s, open(_)).WillByDefault(Return(true));
    
    EXPECT_CALL(conn, open(_, _, _)).Times(1);
    EXPECT_CALL(s, read(_, _))
        .Times(2);
    
    EXPECT_CALL(conn, close(_)).Times(1);

    EXPECT_CALL(conn, write(_, _, 20)).Times(1);
    EXPECT_CALL(conn, write(_, _, 0)).Times(0);
    EXPECT_CALL(s, close()).Times(1);

    EXPECT_TRUE(put_handler(c, &s, &conn));

}

int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



