#include "gtest/gtest.h"
#include "../Reader/WAVReader.h"
#include "../ConfigParser.h"
#include "../MainUtils.h"
#include "../Processor.h"
#include <vector>

namespace{
    TEST(WAVReaderTEST,HeaderCorrect){
        WAVReader wavReader("../src/tests/testFiles/headerWithoutLIST");
        wavReader.readHeader();
    }

    TEST(WAVReaderTEST,HeaderWithLIST){
        WAVReader wavReader("../src/tests/testFiles/headerWithLIST");
        wavReader.readHeader();
    }

    TEST(WAVReaderTEST,CorruptedHeader){
       WAVReader wavReader("../src/tests/testFiles/corruptedHeader");
       EXPECT_ANY_THROW(wavReader.readHeader());
    }

    TEST(WAVReaderTEST,unsupportableFormatHeaderSubChunk1Size){
        WAVReader wavReader("../src/tests/testFiles/unsupportableFormatHeaderSubChunk1Size");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }
    TEST(WAVReaderTEST,unsupportableFormatHeaderFormat){
        WAVReader wavReader("../src/tests/testFiles/unsupportableFormatHeaderFormat");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }
    TEST(WAVReaderTEST,unsupportableFormatHeaderBlockAlign){
        WAVReader wavReader("../src/tests/testFiles/unsupportableFormatHeaderBlockAlign");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }
    TEST(WAVReaderTEST,UnsupportableFormatNumOfChannels){
        WAVReader wavReader("../src/tests/testFiles/unsupportableFormatNumOfChannels");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }

    TEST(ConfigTEST, GoodConfig){
        ConfigParser parser("../src/tests/testFiles/goodConfig.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        parser.parse(config);
    }
    TEST(ConfigTEST, GoodConfigWithLink){
        ConfigParser parser("../src/tests/testFiles/goodConfigWithLink.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        parser.parse(config);
    }
    TEST(ConfigTEST, GoodConfigWithComments){
        ConfigParser parser("../src/tests/testFiles/goodConfigWithComments.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        parser.parse(config);
    }
    TEST(ConfigTEST, BadCommentConfig){
        ConfigParser parser("../src/tests/testFiles/badCommentConfig.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        parser.parse(config);
        std::vector<std::string> files;
        files.push_back("../src/tests/testFiles/headerWithLIST");
        Processor processor;
        EXPECT_ANY_THROW(processor.convert(files,config));
    }

    TEST(ConfigTEST, BadFormatConfig){
        ConfigParser parser("../src/tests/testFiles/badFormatConfig.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        parser.parse(config);
        std::vector<std::string> files;
        files.push_back("../src/tests/testFiles/headerWithLIST");
        files.push_back("../src/tests/testFiles/headerWithLIST");
        Processor processor;
        EXPECT_ANY_THROW(processor.convert(files,config));

    }
    TEST(ConfigTEST, BadLinkConfig){
        ConfigParser parser("../src/tests/testFiles/bigLinkConfig.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        parser.parse(config);
        std::vector<std::string> files;
        files.push_back("../src/tests/testFiles/headerWithLIST");
        files.push_back("../src/tests/testFiles/headerWithLIST");
        Processor processor;
        EXPECT_ANY_THROW(processor.convert(files,config));
    }
    TEST(ConfigTEST, BadLinkConfig1){
        ConfigParser parser("../src/tests/testFiles/badLinkConfig1.txt");
        std::vector<std::variant<std::string,unsigned int>> config;
        EXPECT_ANY_THROW(parser.parse(config));
    }
}