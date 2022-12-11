#include "gtest/gtest.h"
#include "../Reader/WAVReader.h"
#include "../ConfigParser.h"
#include "../MainUtils.h"
#include "../Params/Params.h"
#include <vector>

namespace{
    TEST(WAVReaderTEST,HeaderCorrect){
        WAVReader wavReader(std::string_view(), "../src/tests/testFiles/headerWithoutLIST");
        wavReader.readHeader();
    }

    TEST(WAVReaderTEST,HeaderWithLIST){
        WAVReader wavReader(std::string_view(), "../src/tests/testFiles/headerWithLIST");
        wavReader.readHeader();
    }

    TEST(WAVReaderTEST,CorruptedHeader){
       WAVReader wavReader(std::string_view(), "../src/tests/testFiles/corruptedHeader");
       EXPECT_ANY_THROW(wavReader.readHeader());
    }

    TEST(WAVReaderTEST,unsupportableFormatHeaderSubChunk1Size){
        WAVReader wavReader(std::string_view(), "../src/tests/testFiles/unsupportableFormatHeaderSubChunk1Size");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }
    TEST(WAVReaderTEST,unsupportableFormatHeaderFormat){
        WAVReader wavReader(std::string_view(), "../src/tests/testFiles/unsupportableFormatHeaderFormat");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }
    TEST(WAVReaderTEST,unsupportableFormatHeaderBlockAlign){
        WAVReader wavReader(std::string_view(), "../src/tests/testFiles/unsupportableFormatHeaderBlockAlign");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }
    TEST(WAVReaderTEST,UnsupportableFormatNumOfChannels){
        WAVReader wavReader(std::string_view(), "../src/tests/testFiles/unsupportableFormatNumOfChannels");
        EXPECT_ANY_THROW(wavReader.readHeader());
    }

    TEST(ConfigTEST, GoodConfig){
        ConfigParser parser("../src/tests/testFiles/goodConfig.txt");
        std::vector<std::string> config;
        parser.parse(config);
    }
    TEST(ConfigTEST, GoodConfigWithLink){
        ConfigParser parser("../src/tests/testFiles/goodConfigWithLink.txt");
        std::vector<std::string> config;
        parser.parse(config);
    }
    TEST(ConfigTEST, GoodConfigWithComments){
        ConfigParser parser("../src/tests/testFiles/goodConfigWithComments.txt");
        std::vector<std::string> config;
        parser.parse(config);
    }
    TEST(ConfigTEST, BadLinkConfig1){
        ConfigParser parser("../src/tests/testFiles/badLinkConfig1.txt");
        std::vector<std::string> config;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<Params>> params;
        std::vector<std::string> converters;
        parser.parse(config);
        EXPECT_ANY_THROW(getParamsAndConverters(files,config,params,converters));
    }
    TEST(ConfigTEST, BadLinkConfig2){
        ConfigParser parser("../src/tests/testFiles/badLinkConfig2.txt");
        std::vector<std::string> config;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<Params>> params;
        std::vector<std::string> converters;
        parser.parse(config);
        EXPECT_ANY_THROW(getParamsAndConverters(files,config,params,converters));
    }
    TEST(ConfigTEST, BadCommentConfig){
        ConfigParser parser("../src/tests/testFiles/badCommentConfig.txt");
        std::vector<std::string> config;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<Params>> params;
        std::vector<std::string> converters;
        parser.parse(config);
        EXPECT_ANY_THROW(getParamsAndConverters(files,config,params,converters));
    }
    TEST(ConfigTEST, badFormatConfig){
        ConfigParser parser("../src/tests/testFiles/badFormatConfig.txt");
        std::vector<std::string> config;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<Params>> params;
        std::vector<std::string> converters;
        parser.parse(config);
        EXPECT_ANY_THROW(getParamsAndConverters(files,config,params,converters));
    }
    TEST(ConfigTEST, BigLinkConfig){
        ConfigParser parser("../src/tests/testFiles/bigLinkConfig.txt");
        std::vector<std::string> config;
        std::vector<std::string> files;
        std::vector<std::shared_ptr<Params>> params;
        std::vector<std::string> converters;
        parser.parse(config);
        EXPECT_ANY_THROW(getParamsAndConverters(files,config,params,converters));
    }
}