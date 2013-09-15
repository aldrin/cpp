/// Copyright (c) 2013, Aldrin's Notebook.
/// http://opensource.org/licenses/BSD-2-Clause

#include "crypto.h"

#include <sstream>
#include <algorithm>

using namespace ajd;

void unhexlify(const std::string &hex, std::vector<unsigned char> &bytes)
{
  for (std::size_t i = 0; i < hex.size(); i += 2)
  {
    int byte;
    std::stringstream ss;
    ss << std::hex << hex[i] << hex[i + 1];
    ss >> byte;
    bytes.push_back(byte);
  }
}

void pbkdf_tests()
{
  // test data from: http://tools.ietf.org/html/rfc6070 (ignoring the really slow one c=16777216)
  // expected values from: http://stackoverflow.com/questions/5130513/pbkdf2-hmac-sha2-test-vectors
  struct test
  {
    int c;               // iteration count
    int dkLen;           // derived key length
    std::string P;       // password
    std::string S;       // salt
    std::string DK;      // expected output: derived key
  } tests[5];

  tests[0].c = 1;
  tests[0].dkLen = 32;
  tests[0].S = "salt";
  tests[0].P = "password";
  tests[0].DK = "120fb6cffcf8b32c43e7225256c4f837a86548c92ccc35480805987cb70be17b";

  tests[1].c = 2;
  tests[1].dkLen = 32;
  tests[1].S = "salt";
  tests[1].P = "password";
  tests[1].DK = "ae4d0c95af6b46d32d0adff928f06dd02a303f8ef3c251dfd6e2d85a95474c43";

  tests[2].c = 4096;
  tests[2].dkLen = 32;
  tests[2].S = "salt";
  tests[2].P = "password";
  tests[2].DK = "c5e478d59288c841aa530db6845c4c8d962893a001ce4e11a4963873aa98134a";

  tests[3].c = 4096;
  tests[3].dkLen = 40;
  tests[3].S = "saltSALTsaltSALTsaltSALTsaltSALTsalt";
  tests[3].P = "passwordPASSWORDpassword";
  tests[3].DK = "348c89dbcbd32b2f32d814b8116e84cf2b17347ebc1800181c4e2a1fb8dd53e1c635518c7dac47e9";

  tests[4].c = 4096;
  tests[4].dkLen = 16;
  tests[4].S = std::string("sa\0lt", 5);
  tests[4].P = std::string("pass\0word", 9);
  tests[4].DK = "89b69d0516f829893c696226650a8687";

  for (size_t i = 0; i < sizeof(tests) / sizeof(struct test); i++)
  {
    std::vector<unsigned char> expected;
    std::vector<unsigned char> key(tests[i].dkLen);
    unhexlify(tests[i].DK, expected);
    crypto::derive_key(key, tests[i].P, tests[i].S, tests[i].c);
    assert(std::equal(expected.begin(), expected.end(), key.begin()));
  }
}

void sha256_tests()
{
  // Test vectors from: http://csrc.nist.gov/groups/STM/cavp/index.html#03
  // Only a few test cases were chosen since we're only doing sanity tests here.
  // In particular, we've chosen Len=0, 8, 64, 512 from SHA256ShortMsg.rsp
  struct test
  {
    std::string Msg;     // message
    std::string MD;      // digest
  } tests[4];

  tests[0].Msg = "";
  tests[0].MD = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

  tests[1].Msg = "d3";
  tests[1].MD = "28969cdfa74a12c82f3bad960b0b000aca2ac329deea5c2328ebc6f2ba9802c1";

  tests[2].Msg = "5738c929c4f4ccb6";
  tests[2].MD = "963bb88f27f512777aab6c8b1a02c70ec0ad651d428f870036e1917120fb48bf";

  tests[3].Msg = "5a86b737eaea8ee976a0a24da63e7ed7eefad18a101c1211e2b3650c5187c2a"
                 "8a650547208251f6d4237e661c7bf4c77f335390394c37fa1a9f9be836ac28509";
  tests[3].MD = "42e61e174fbb3897d6dd6cef3dd2802fe67b331953b06114a65c772859dfc1aa";

  crypto::hash md;

  for (size_t i = 0; i < sizeof(tests) / sizeof(struct test); i++)
  {
    crypto::hash::value value;
    std::vector<unsigned char> message;
    std::vector<unsigned char> expected;
    unhexlify(tests[i].Msg, message);
    unhexlify(tests[i].MD, expected);
    md.update(message);
    md.finalize(value);
    assert(std::equal(expected.begin(), expected.end(), value.begin()));
  }
}

void hmac_tests()
{
  // test data from https://tools.ietf.org/html/rfc4231
  struct test
  {
    std::string Key;
    std::string Data;
    std::string MAC;
  } tests[6];

  tests[0].Key = "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b";
  tests[0].Data = "4869205468657265";
  tests[0].MAC = "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7";

  tests[1].Key = "4a656665";
  tests[1].Data = "7768617420646f2079612077616e7420666f72206e6f7468696e673f";
  tests[1].MAC = "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843";

  tests[2].Key = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  tests[2].Data = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"
                  "dddddddddddddddddddddddddddddddd";
  tests[2].MAC = "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe";

  tests[3].Key = "0102030405060708090a0b0c0d0e0f10111213141516171819";
  tests[3].Data = "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd"
                  "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd";
  tests[3].MAC = "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b";
  tests[4].Key = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaa";
  tests[4].Data = "54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a"
                  "65204b6579202d2048617368204b6579204669727374";
  tests[4].MAC = "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54";

  tests[5].Key = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                 "aaaaaa";
  tests[5].Data = "5468697320697320612074657374207573696e672061206c6172676572207468"
                  "616e20626c6f636b2d73697a65206b657920616e642061206c61726765722074"
                  "68616e20626c6f636b2d73697a6520646174612e20546865206b6579206e6565"
                  "647320746f20626520686173686564206265666f7265206265696e6720757365"
                  "642062792074686520484d414320616c676f726974686d2e";
  tests[5].MAC = "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2";

  for (size_t i = 0; i < sizeof(tests) / sizeof(struct test); i++)
  {
    std::vector<unsigned char> key;
    std::vector<unsigned char> data;
    std::vector<unsigned char> expected;
    unhexlify(tests[i].Data, data);
    unhexlify(tests[i].Key, key);
    unhexlify(tests[i].MAC, expected);
    crypto::hash h(key);
    crypto::hash::value mac;
    h.update(data).finalize(mac);
    assert(std::equal(expected.begin(), expected.end(), mac.begin()));
  }
}

void gcm_tests()
{
  // test data from CryptoC++ Test Vectors (gcm.txt)
  // google?q="Source: aes-modes-src-07-10-08/Testvals/gcm.1, Basic Tests for GCM (compiled by B. R. Gladman)"
  struct test
  {
    std::string k;
    std::string iv;
    std::string aad;
    std::string tag;
    std::string text;
    std::string ctext;
  } tests[5];

  tests[0].k = "00000000000000000000000000000000";
  tests[0].iv = "000000000000000000000000";
  tests[0].tag = "ab6e47d42cec13bdf53a67b21257bddf";
  tests[0].text = "00000000000000000000000000000000";
  tests[0].ctext = "0388dace60b6a392f328c2b971b2fe78";

  tests[1].k = "feffe9928665731c6d6a8f9467308308";
  tests[1].iv = "cafebabefacedbaddecaf888";
  tests[1].tag = "4d5c2af327cd64a62cf35abd2ba6fab4";
  tests[1].text = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
                  "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255";
  tests[1].ctext = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
                   "21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985";

  tests[2].k = "feffe9928665731c6d6a8f9467308308";
  tests[2].iv = "cafebabefacedbaddecaf888";
  tests[2].aad = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
  tests[2].tag = "5bc94fbc3221a5db94fae95ae7121a47";
  tests[2].text = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
                  "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
  tests[2].ctext = "42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e"
                   "21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091";

  tests[3].k = "feffe9928665731c6d6a8f9467308308";
  tests[3].iv = "cafebabefacedbad";
  tests[3].aad = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
  tests[3].tag = "3612d2e79e3b0785561be14aaca2fccb";
  tests[3].text = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
                  "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
  tests[3].ctext = "61353b4c2806934a777ff51fa22a4755699b2a714fcdc6f83766e5f97b6c7423"
                   "73806900e49f24b22b097544d4896b424989b5e1ebac0f07c23f4598";

  tests[4].k = "feffe9928665731c6d6a8f9467308308";
  tests[4].iv = "9313225df88406e555909c5aff5269aa6a7a9538534f7da1e4c303d2a318a728"
                "c3c0c95156809539fcf0e2429a6b525416aedbf5a0de6a57a637b39b";
  tests[4].aad = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
  tests[4].tag = "619cc5aefffe0bfa462af43c1699d050";
  tests[4].text = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a72"
                  "1c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
  tests[4].ctext = "8ce24998625615b603a033aca13fb894be9112a5c3a211a8ba262a3cca7e2ca7"
                   "01e4a9a4fba43c90ccdcb281d48c7c6fd62875d2aca417034c34aee5";

  for (size_t i = 0; i < sizeof(tests) / sizeof(struct test); i++)
  {
    std::vector<unsigned char> iv;
    std::vector<unsigned char> key;
    std::vector<unsigned char> aad;
    std::vector<unsigned char> tag;
    std::vector<unsigned char> text;
    std::vector<unsigned char> ctext;
    unhexlify(tests[i].k, key);
    unhexlify(tests[i].iv, iv);
    unhexlify(tests[i].aad, aad);
    unhexlify(tests[i].tag, tag);
    unhexlify(tests[i].text, text);
    unhexlify(tests[i].ctext, ctext);

    crypto::block seal;
    std::vector<unsigned char> enc(text.size());
    std::vector<unsigned char> dec(text.size());
    {
      crypto::cipher cipher(key, iv);
      cipher.associate_data(aad);
      cipher.transform(text, enc);
      cipher.seal(seal);
      assert(std::equal(enc.begin(), enc.end(), ctext.begin()));
      assert(std::equal(seal.begin(), seal.end(), tag.begin()));
    }
    {
      crypto::cipher cipher(key, iv, seal);
      cipher.associate_data(aad);
      cipher.transform(enc, dec);
      cipher.verify();
      assert(std::equal(dec.begin(), dec.end(), text.begin()));
    }

  }
}

int main()
{
  gcm_tests();
  hmac_tests();
  pbkdf_tests();
  sha256_tests();
}
