#include "../BigInt.h"
#include "gtest/gtest.h"

namespace {

    TEST(ConstructorsTest,voidConstructor){
        BigInt x;
        EXPECT_EQ(0,x.numberDigits(0));
        EXPECT_EQ(1,x.countOfDigits());
        EXPECT_EQ(true,x.isPositive());
    }

    TEST(ConstructorsTest,longConstructor){
        BigInt positive = BigInt(123456); // 1*256^2+226*256+64
        EXPECT_EQ(64,positive.numberDigits(0));
        EXPECT_EQ(226,positive.numberDigits(1));
        EXPECT_EQ(1,positive.numberDigits(2));
        EXPECT_EQ(3,positive.countOfDigits());
        EXPECT_EQ(true, positive.isPositive());

        BigInt negative = BigInt(-123456);
        EXPECT_EQ(64,positive.numberDigits(0));
        EXPECT_EQ(226,positive.numberDigits(1));
        EXPECT_EQ(1,positive.numberDigits(2));
        EXPECT_EQ(3,negative.countOfDigits());
        EXPECT_EQ(false, negative.isPositive());

        BigInt zero = BigInt(0);
        EXPECT_EQ(0,zero.numberDigits(0));
        EXPECT_EQ(1,zero.countOfDigits());
        EXPECT_EQ(true, zero.isPositive());
    }

    TEST(ConstructorsTest,stringConstructor){
        EXPECT_ANY_THROW(BigInt("abcd"));
        EXPECT_ANY_THROW(BigInt("124532a"));
        EXPECT_ANY_THROW(BigInt("124a532"));
        EXPECT_ANY_THROW(BigInt("-"));
        EXPECT_ANY_THROW(BigInt("--"));
        EXPECT_ANY_THROW(BigInt("-a"));
        EXPECT_ANY_THROW(BigInt("-215-24"));
        EXPECT_ANY_THROW(BigInt(""));
        EXPECT_ANY_THROW(BigInt(" "));

        EXPECT_TRUE(BigInt("0") == BigInt());
        EXPECT_TRUE(BigInt("-0") == BigInt());
        EXPECT_TRUE(BigInt("1234567890") == BigInt(1234567890));
        EXPECT_TRUE(BigInt("00123456") == BigInt(123456));
        EXPECT_TRUE(BigInt("-00123456") == BigInt(-123456));
        EXPECT_FALSE(BigInt("-123456") == BigInt(123456));

        BigInt l("21345817372864405881847059188222722561"); // 1+2*2^8+3*2^16 + ... + 16*2^120
        for (int i = 0; i < 16; ++i) {
            EXPECT_TRUE(l.numberDigits(i) == i+1);
        }

    }

    TEST(ConstructorsTest,copyConstructor){
        BigInt base("12345678901234567890");
        BigInt copyed(base);
        EXPECT_TRUE(copyed == base);
        BigInt negBase("-12345678901234567890");
        BigInt negCopyed(negBase);
        EXPECT_TRUE(negBase == negCopyed);
    }

    TEST(CmpOpTest,diffSgn){
        EXPECT_TRUE(BigInt(2) > BigInt(-1));
        EXPECT_FALSE(BigInt(-2) > BigInt(1));

        EXPECT_TRUE(BigInt(2) >= BigInt(-1));
        EXPECT_FALSE(BigInt(-2) >= BigInt(1));

        EXPECT_FALSE(BigInt(2) <= BigInt(-1));
        EXPECT_TRUE(BigInt(-2) <= BigInt(1));

        EXPECT_FALSE(BigInt(2) < BigInt(-1));
        EXPECT_TRUE(BigInt(-2) < BigInt(1));
    }

    TEST(CmpOpTest, equalNumbers){
        EXPECT_FALSE(BigInt(1) > BigInt(1));
        EXPECT_FALSE(BigInt(1) < BigInt(1));
        EXPECT_TRUE(BigInt(1) >= BigInt(1));
        EXPECT_TRUE(BigInt(1) <= BigInt(1));
    }

    TEST(CmpOpTest,diffNumbers){
        EXPECT_TRUE(BigInt(2) > BigInt(1));
        EXPECT_TRUE(BigInt(23) > BigInt(18));
        EXPECT_TRUE(BigInt(3310) >= BigInt(1011));
        EXPECT_FALSE(BigInt(3310) < BigInt(1011));
        EXPECT_TRUE(BigInt(3338) < BigInt(3361));
        EXPECT_FALSE(BigInt(3381) < BigInt(3369));
        EXPECT_TRUE(BigInt(1234567890) < BigInt(1234567899));
        EXPECT_TRUE(BigInt(1234567890) < BigInt(1235537899));
        EXPECT_TRUE(BigInt(1) < BigInt(2));
        EXPECT_FALSE(BigInt(-2) > BigInt(-1));
        EXPECT_FALSE(BigInt(-1) < BigInt(-2));

        EXPECT_TRUE(BigInt(2) >= BigInt(1));
        EXPECT_TRUE(BigInt(1) <= BigInt(2));
        EXPECT_FALSE(BigInt(-2) >= BigInt(-1));
        EXPECT_FALSE(BigInt(-1) <= BigInt(-2));
    }

    TEST(CmpOpTest,equalityOperators){
        EXPECT_TRUE(BigInt(1) == BigInt(1));
        EXPECT_FALSE(BigInt(1) == BigInt(2));
        EXPECT_TRUE(BigInt("123456789123456789123456789") == BigInt("123456789123456789123456789"));
        EXPECT_FALSE(BigInt("123456789123456789123456789") == BigInt("123456789123456789123456780"));
        EXPECT_TRUE(BigInt(1) != BigInt(2));
        EXPECT_FALSE(BigInt(1) != BigInt(1));
        EXPECT_FALSE(BigInt(-1) == BigInt(1));
        EXPECT_TRUE(BigInt(-1) != BigInt(1));
    }

    TEST(SimpleOperatorsTest,operatorAssignment){
        BigInt base("12345678901234567890");
        BigInt copyed;
        copyed = base;
        EXPECT_TRUE(copyed == base);
        BigInt negBase("-12345678901234567890");
        BigInt negCopyed;
        negCopyed = negBase;
        EXPECT_TRUE(negBase == negCopyed);
    }

    TEST(SimpleOperatorsTest,unaryOperators){
        BigInt uP(123456);
        BigInt uN(-123456);
        EXPECT_TRUE(+uP == BigInt(123456));
        EXPECT_TRUE(+uN == BigInt(-123456));
        EXPECT_TRUE(-uP == BigInt(-123456));
        EXPECT_TRUE(-uN == BigInt(123456));
    }

    TEST(AdditionOperatorTest, equalSigns){
        BigInt a(1);
        a+= BigInt(3);
        EXPECT_TRUE(a == BigInt(4));
        EXPECT_TRUE(BigInt(1) + BigInt(3) == BigInt(4));

        BigInt aC(99999999);
        aC+=BigInt(1);
        EXPECT_TRUE(aC == BigInt(100000000));
        EXPECT_TRUE(BigInt(99999999) + BigInt(1) == BigInt(100000000));

        BigInt aZ;
        aZ+=BigInt(42);
        EXPECT_TRUE(aZ == BigInt(42));
        EXPECT_TRUE(BigInt() + BigInt(42) == BigInt(42));

        BigInt aZZ;
        aZZ+= BigInt();
        EXPECT_TRUE(aZZ == BigInt());
        EXPECT_TRUE(BigInt() + BigInt() == BigInt());

        BigInt oC(123499);
        oC+=BigInt(1);
        EXPECT_TRUE(oC == BigInt(123500));
        EXPECT_TRUE(BigInt(123499) + BigInt(1) == BigInt(123500));

        BigInt p1(123);
        BigInt p2(234);
        p1+=p2;
        p2+=BigInt(123);
        EXPECT_TRUE(p1 == p2);
        EXPECT_TRUE(BigInt(123) + BigInt(234) == BigInt(234) + BigInt(123));

        BigInt big("1234567898765432112345678987654321");
        big+= BigInt("111111111111111111111111111111111111");
        EXPECT_TRUE(big == BigInt("112345679009876543223456790098765432"));
        EXPECT_TRUE(BigInt("112345679009876543223456790098765432") == BigInt("1234567898765432112345678987654321") + BigInt("111111111111111111111111111111111111"));

        BigInt aN(-1);
        aN+= BigInt(-3);
        EXPECT_TRUE(aN == BigInt(-4));
        EXPECT_TRUE(BigInt(-1) + BigInt(-3) == BigInt(-4));

        BigInt aCN(-99999999);
        aCN+=BigInt(-1);
        EXPECT_TRUE(aCN == BigInt(-100000000));
        EXPECT_TRUE(BigInt(-99999999) + BigInt(-1) == BigInt(-100000000));

        BigInt aZN(-0);
        aZN+=BigInt(-42);
        EXPECT_TRUE(aZN == BigInt(-42));
        EXPECT_TRUE(BigInt(-0) + BigInt(-42) == BigInt(-42));

        BigInt aZZN(-0);
        aZZN+= BigInt(-0);
        EXPECT_TRUE(aZZN == BigInt(-0));
        EXPECT_TRUE(BigInt(-0) + BigInt(-0) == BigInt(-0));

        BigInt oCN(-123499);
        oCN+=BigInt(-1);
        EXPECT_TRUE(oCN == BigInt(-123500));
        EXPECT_TRUE(BigInt(-123499) + BigInt(-1) == BigInt(-123500));

        BigInt p1N(-123);
        BigInt p2N(-234);
        p1N+=p2N;
        p2N+=BigInt(-123);
        EXPECT_TRUE(p1N == p2N);
        EXPECT_TRUE(BigInt(-123) + BigInt(-234) == BigInt(-234) + BigInt(-123));

        BigInt bigN("-1234567898765432112345678987654321");
        bigN+= BigInt("-111111111111111111111111111111111111");
        EXPECT_TRUE(bigN == BigInt("-112345679009876543223456790098765432"));
        EXPECT_TRUE(BigInt("-112345679009876543223456790098765432") == BigInt("-1234567898765432112345678987654321") + BigInt("-111111111111111111111111111111111111"));
    }

    TEST(SubstractionOperatorTest, equalSigns){
        BigInt aM(10);
        aM -= BigInt(3);
        EXPECT_TRUE(aM == BigInt(7));
        EXPECT_TRUE(BigInt(10) - BigInt(3) == BigInt(7));

        BigInt aL(10);
        aL -= BigInt(18);
        EXPECT_TRUE(aL == BigInt(-8));
        EXPECT_TRUE(BigInt(10) - BigInt(18) == BigInt(-8));

        BigInt aZ;
        aZ -= BigInt(18);
        EXPECT_TRUE(aZ == BigInt(-18));
        EXPECT_TRUE(BigInt() - BigInt(18) == BigInt(-18));

        BigInt aE(42);
        aE -= BigInt(42);
        EXPECT_TRUE(aE == BigInt());
        EXPECT_TRUE(BigInt(42) - BigInt(42) == BigInt());

        BigInt aC(10000000);
        aC -= BigInt(1);
        EXPECT_TRUE(aC == BigInt(9999999));
        EXPECT_TRUE(BigInt(10000000) - BigInt(1) == BigInt(9999999));

        BigInt aC2(12345000);
        aC2 -= BigInt(1);
        EXPECT_TRUE(aC2 == BigInt(12344999));
        EXPECT_TRUE(BigInt(12345000) - BigInt(1) == BigInt(12344999));

        BigInt aP1(12345);
        BigInt aP2(45);
        aP1 -= BigInt(45);
        aP2 -= BigInt(12345);
        EXPECT_TRUE(aP1 == -aP2);
        EXPECT_TRUE(BigInt(123) - BigInt(42) == -(BigInt(42) - BigInt(123)));


        BigInt big("491285793286394792386935783495719329962");
        big -= BigInt("3082498158249683948293856749363634");
        EXPECT_TRUE(big == BigInt("491282710788236542702987489638969966328"));
        EXPECT_TRUE(BigInt("491282710788236542702987489638969966328") == BigInt("491285793286394792386935783495719329962") - BigInt("3082498158249683948293856749363634"));

        BigInt aMN(-10);
        aMN -= BigInt(-3);
        EXPECT_TRUE(aMN == BigInt(-7));
        EXPECT_TRUE(BigInt(-10) - BigInt(-3) == BigInt(-7));

        BigInt aLN(-10);
        aLN -= BigInt(-18);
        EXPECT_TRUE(aLN == BigInt(8));
        EXPECT_TRUE(BigInt(-10) - BigInt(-18) == BigInt(8));

        BigInt aZN(-0);
        aZN -= BigInt(-18);
        EXPECT_TRUE(aZN == BigInt(18));
        EXPECT_TRUE(BigInt(-0) - BigInt(-18) == BigInt(18));

        BigInt aEN(-42);
        aEN -= BigInt(-42);
        EXPECT_TRUE(aEN == BigInt());
        EXPECT_TRUE(BigInt(-42) - BigInt(-42) == BigInt());

        BigInt aCN(-10000000);
        aCN -= BigInt(-1);
        EXPECT_TRUE(aCN == BigInt(-9999999));
        EXPECT_TRUE(BigInt(-10000000) - BigInt(-1) == BigInt(-9999999));

        BigInt aC2N(-12345000);
        aC2N -= BigInt(-1);
        EXPECT_TRUE(aC2N == BigInt(-12344999));
        EXPECT_TRUE(BigInt(-12345000) - BigInt(-1) == BigInt(-12344999));

        BigInt aP1N(-12345);
        BigInt aP2N(-45);
        aP1N -= BigInt(-45);
        aP2N -= BigInt(-12345);
        EXPECT_TRUE(aP1N == -aP2N);
        EXPECT_TRUE(BigInt(-123) - BigInt(-42) == -(BigInt(-42) - BigInt(-123)));

        BigInt bigN("-491285793286394792386935783495719329962");
        bigN -= BigInt("-3082498158249683948293856749363634");
        EXPECT_TRUE(bigN == BigInt("-491282710788236542702987489638969966328"));
        EXPECT_TRUE(BigInt("-491282710788236542702987489638969966328") == BigInt("-491285793286394792386935783495719329962") - BigInt("-3082498158249683948293856749363634"));
    }

    TEST(AdditionOperatorTest, differentSigns){
        BigInt aP(154);
        aP += BigInt(-54);
        EXPECT_TRUE(aP == BigInt(100));
        EXPECT_TRUE(BigInt(154) + BigInt(-54) == BigInt(100));

        BigInt aN(-154);
        aN+=BigInt(54);
        EXPECT_TRUE(aN == BigInt(-100));
        EXPECT_TRUE(BigInt(-154) + BigInt(54) == BigInt(-100));

        BigInt aE(122);
        aE+=BigInt(-122);
        EXPECT_TRUE(aE == BigInt());
        EXPECT_TRUE(BigInt(42) + BigInt(-42) == BigInt());

        BigInt aZ;
        aZ+= BigInt(-0);
        EXPECT_TRUE(aZ == BigInt());
        EXPECT_TRUE(BigInt() + BigInt(-0) == BigInt());
    }

    TEST(SubstractionOperatorTest,differentSigns){
        BigInt aP(144);
        aP -= BigInt(-44);
        EXPECT_TRUE(aP == BigInt(188));
        EXPECT_TRUE(BigInt(144) - BigInt(-44) == BigInt(188));

        BigInt aN(-144);
        aN -= BigInt(44);
        EXPECT_TRUE(aN == BigInt(-188));
        EXPECT_TRUE(BigInt(-144) - BigInt(44) == BigInt(-188));

        BigInt aZ;
        aZ-=BigInt(-0);
        EXPECT_TRUE(aZ == BigInt());
        EXPECT_TRUE(BigInt() - BigInt(-0) == BigInt());

        BigInt eQ(100);
        eQ -= BigInt(-100);
        EXPECT_TRUE(eQ == BigInt(200));
        EXPECT_TRUE(BigInt(100) - BigInt(-100) == BigInt(200));
    }

    TEST(SimpleOperatorsTest, incrementOperator){
        BigInt a(19);
        a++;
        EXPECT_TRUE(a == BigInt(20));
        ++a;
        EXPECT_TRUE(a == BigInt(21));
        EXPECT_TRUE(a++ == BigInt(21));
        EXPECT_TRUE(++a == BigInt(23));
    }

    TEST(SimpleOperatorTest, decrementOperator){
        BigInt b(10);
        b--;
        EXPECT_TRUE(b == BigInt(9));
        --b;
        EXPECT_TRUE(b == BigInt(8));
        EXPECT_TRUE(b-- == BigInt(8));
        EXPECT_TRUE(--b == BigInt(6));
    }

    TEST(MultiplyOperatorTest, positiveSign){
        BigInt aZ(135);
        aZ*=BigInt();
        EXPECT_TRUE(aZ == BigInt());
        EXPECT_TRUE(BigInt(134) * BigInt() ==  BigInt());

        BigInt aO(103);
        aO*= BigInt(1);
        EXPECT_TRUE(aO == aO);
        EXPECT_TRUE(BigInt(135) * BigInt(1) == BigInt(135));

        BigInt aP1(12);
        BigInt aP2(4);
        aP1 *= BigInt(4);
        aP2 *= BigInt(12);
        EXPECT_TRUE(aP1 == aP2);
        EXPECT_TRUE(BigInt(4) * BigInt(12) == BigInt(12) * BigInt(4));

        BigInt big("83476934867934859236943769348762068430735");
        big*= BigInt("938467934867296834769283697429867329867349768349682479673489672469824879624986724");
        EXPECT_TRUE(big == BigInt("78340426674562671581876992334659393176126608992909608800334917125305465287538243764199592749506943646674209860053888562140"));
        EXPECT_TRUE(BigInt("78340426674562671581876992334659393176126608992909608800334917125305465287538243764199592749506943646674209860053888562140") ==
        BigInt("83476934867934859236943769348762068430735") * BigInt("938467934867296834769283697429867329867349768349682479673489672469824879624986724"));



    }

    TEST(MultiplyOperatorTest, signs){
        BigInt aPN(52);
        aPN *= BigInt(-2);
        EXPECT_TRUE(aPN == BigInt(-104));
        EXPECT_TRUE(BigInt(52) * BigInt(-2) == BigInt(-104));

        BigInt aNP(-52);
        aNP *= BigInt(2);
        EXPECT_TRUE(aNP == BigInt(-104));
        EXPECT_TRUE(BigInt(-52) * BigInt(2) == BigInt(-104));

        BigInt aNN(-52);
        aNN *= BigInt(-2);
        EXPECT_TRUE(aNN == BigInt(104));
        EXPECT_TRUE(BigInt(-52) * BigInt(-2) == BigInt(104));
    }

    TEST(DivisionOperatorTest, positiveSign){
        BigInt a(24);
        EXPECT_ANY_THROW(a/=BigInt());
        EXPECT_ANY_THROW(BigInt(25) / BigInt());

        BigInt aM(62);
        aM /= BigInt(10);
        EXPECT_TRUE(aM == BigInt(6));
        EXPECT_TRUE(BigInt(62) / BigInt(6) == BigInt(10));

        BigInt aL(35);
        aL /= BigInt(36);
        EXPECT_TRUE(aL == BigInt());
        EXPECT_TRUE(BigInt(35) / BigInt(36) == BigInt());

        BigInt aO(12345);
        aO /= BigInt(1);
        EXPECT_TRUE(aO == aO);
        EXPECT_TRUE(BigInt(12345) / BigInt(1) == BigInt(12345));

        BigInt aE(1234);
        aE /= BigInt(1234);
        EXPECT_TRUE(aE == BigInt(1));
        EXPECT_TRUE(BigInt(12345) / BigInt(12345) == BigInt(1));

        BigInt aZ;
        aZ /= BigInt(56);
        EXPECT_TRUE(aZ == BigInt());
        EXPECT_TRUE(BigInt() / BigInt(52) == BigInt());

        BigInt big1("3298572396729385792836732867932845983468239867349867248967824895672359348763789468238976284967248964987624886498763489672486924896248798432986");
        big1 /= BigInt(123456);
        EXPECT_TRUE(big1 == BigInt("26718607412595465532956947154717842660285768754453953221940002070959364864921830192448939581448037883842218170836277618523902644636540941"));
        EXPECT_TRUE(BigInt("26718607412595465532956947154717842660285768754453953221940002070959364864921830192448939581448037883842218170836277618523902644636540941")
        == BigInt("3298572396729385792836732867932845983468239867349867248967824895672359348763789468238976284967248964987624886498763489672486924896248798432986") / BigInt(123456));

        BigInt big2("3298572395832796284679384769348769238639487635896724896739876298467928376892735892738519305691368349867132896739487691386938276348682939681036092368942378634");
        big2 /= BigInt("1205984296834097834097860236832946705398703985072305870639468304297039870693248502386049870268932409782409680924097345");
        EXPECT_TRUE(big2 == BigInt("2735170270866774774205310395793763467401"));
        EXPECT_TRUE(BigInt("2735170270866774774205310395793763467401") == BigInt("3298572395832796284679384769348769238639487635896724896739876298467928376892735892738519305691368349867132896739487691386938276348682939681036092368942378634")
        / BigInt("1205984296834097834097860236832946705398703985072305870639468304297039870693248502386049870268932409782409680924097345"));
    }

    TEST(DivisionOperatorTest, signs){
        BigInt PN(360);
        PN /= BigInt(-12);
        EXPECT_TRUE(PN == BigInt(-30));
        EXPECT_TRUE(BigInt(360) / BigInt(-12) == BigInt(-30));

        BigInt NP(-330);
        NP /= BigInt(10);
        EXPECT_TRUE(NP == BigInt(-33));
        EXPECT_TRUE(BigInt(-330) / BigInt(10) == BigInt(-33));

        BigInt NN(-48);
        NN /= BigInt(-6);
        EXPECT_TRUE(NN == BigInt(8));
        EXPECT_TRUE(BigInt(-48) / BigInt(-6) == BigInt(8));
    }

    TEST(ModOperatorTest, positiveSign){
        BigInt ex(3);
        EXPECT_ANY_THROW(ex%=BigInt());
        EXPECT_ANY_THROW(BigInt(5) % BigInt());

        BigInt aM(523);
        aM %= BigInt(13);
        EXPECT_TRUE(aM == BigInt(3));
        EXPECT_TRUE(BigInt(523) % BigInt(13) == BigInt(3));

        BigInt aL(1566);
        aL %= BigInt(3000);
        EXPECT_TRUE(aL == aL);
        EXPECT_TRUE(BigInt(1566) % BigInt(3000) == BigInt(1566));

        BigInt aZ;
        aZ %= BigInt(56);
        EXPECT_TRUE(aZ == BigInt());
        EXPECT_TRUE(BigInt() % BigInt(52) == BigInt());

        BigInt aE(52);
        aE %= BigInt(52);
        EXPECT_TRUE(aE == BigInt());
        EXPECT_TRUE(BigInt(35) % BigInt(35) == BigInt());

        BigInt big1("82179587329865734896782935823769835789483562937629634807240598329582039672497635896882039520398560276358783406012390582960347349638497580397935");
        big1 %= BigInt("12345678987654321");
        EXPECT_TRUE(big1 == BigInt("9924315271755804"));
        EXPECT_TRUE(BigInt("82179587329865734896782935823769835789483562937629634807240598329582039672497635896882039520398560276358783406012390582960347349638497580397935")
        % BigInt("12345678987654321") == BigInt("9924315271755804"));

        BigInt big2("293850394680429860395780295820360437613209685204968593807890240962930683468903487962497609328762834683468450760246982412341252352532");
        big2 %= BigInt("9036830497803497803948602396893497590783409693046059230896824079635906893290683407820496824073890708240968024");
        EXPECT_TRUE(big2 == BigInt("4627748142955675988553761222888091353414084611285965899882812033328555160800734690640797516364922149067778732"));
        EXPECT_TRUE(BigInt("4627748142955675988553761222888091353414084611285965899882812033328555160800734690640797516364922149067778732")
        == BigInt("293850394680429860395780295820360437613209685204968593807890240962930683468903487962497609328762834683468450760246982412341252352532") %
           BigInt("9036830497803497803948602396893497590783409693046059230896824079635906893290683407820496824073890708240968024"));
    }

    TEST(ModOperatorTest, signs){
        BigInt PN(362);
        PN %= BigInt(-12);
        EXPECT_TRUE(PN == BigInt(2));
        EXPECT_TRUE(BigInt(362) % BigInt(-12) == BigInt(2));

        BigInt NP(-330);
        NP %= BigInt(16);
        EXPECT_TRUE(NP == BigInt(6));
        EXPECT_TRUE(BigInt(-330) % BigInt(16) == BigInt(6));

        BigInt NN(-48);
        NN %= BigInt(-9);
        EXPECT_TRUE(NN == BigInt(6));
        EXPECT_TRUE(BigInt(-48) % BigInt(-6) == BigInt(6));
    }

    TEST(SimpleOperatorTest, toIntOperator){
        BigInt sP(36);
        int ap = int(sP);
        EXPECT_TRUE(ap == 36);

        BigInt sN(-44);
        int an = int(sN);
        EXPECT_TRUE(an == -44);

        BigInt bP("123456789123456789");
        int bp = int(bP);
        EXPECT_TRUE(bp == -1395630315);

        BigInt bN("-123456789123456789");
        int bn = int(bN);
        EXPECT_TRUE(bn == 1395630315);
    }

    TEST(IOOperatorsTest,toStringOperator){
        BigInt a0;
        EXPECT_TRUE(std::string(a0) == "0");

        BigInt sP(12345);
        EXPECT_TRUE(std::string(sP) == "12345");

        BigInt sN(-12345);
        EXPECT_TRUE(std::string(sN) == "-12345");

        BigInt bP("1234567898765432112345678909876543210");
        EXPECT_TRUE(std::string(bP) == "1234567898765432112345678909876543210");

        BigInt bN("-1234567898765432112345678909876543210");
        EXPECT_TRUE(std::string(bN) == "-1234567898765432112345678909876543210");
    }


}
