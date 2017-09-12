//
//  ZDZLSearchUtil.cpp
//  ZDStudent
//
//  Created by 蒋晨霄 on 2016/12/26.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLSearchUtil.hpp"

#include "ZLUnicodeUtil.h"

ZDZLSearchPattern::ZDZLSearchPattern(std::string pattern, long length, bool ignoreCase) {
    IgnoreCase = ignoreCase;
    patternLength = length;

    std::string str = "\u200b";
    long pos = pattern.find(str);
    if (pos >= 0) {
        pattern = pattern.replace(pos, str.length(), "");
    }
    if (IgnoreCase) {
        LowerCasePattern = pattern;
        UpperCasePattern = pattern;
        std::transform(LowerCasePattern.begin(), LowerCasePattern.end(), LowerCasePattern.begin(), ::tolower);
        std::transform(UpperCasePattern.begin(), UpperCasePattern.end(), UpperCasePattern.begin(), ::toupper);
    } else {
        LowerCasePattern = pattern;
    }
}

shared_ptr<ZDZLSearchUtil::Result> ZDZLSearchUtil::find(short* textData, int offset, int length,
                                                        shared_ptr<ZDZLSearchPattern> pattern, int pos) {
    if (pos < 0) {
        pos = 0;
    }
    int searchStartPos = offset + pos;

    std::string lower = pattern->lowerCasePattern();
    int patternLength = (int)lower.length();
//    printf("pattern lower: %s, length: %d\n", lower.c_str(), patternLength);
//    printf("pattern code: ");
//    for (int i = 0; i < patternLength; i++) {
//        printf("%c ", lower[i]);
//    }
//    printf("\n");

    // 将short类型的textData转换成utf8的字符串
    std::string sourceText = "";
    ZLUnicodeUtil::Ucs2String sourceData;
    for (int i = 0; i < length; i++) {
        sourceData.insert(sourceData.end(), textData[searchStartPos + i]);
    }
    ZLUnicodeUtil::ucs2ToUtf8(sourceText, sourceData);
//    printf("sourceText: %s, length: %ld\n", sourceText.c_str(), sourceText.length());
//    printf("sourceText code: ");
//    for (int i = 0; i < sourceText.length(); i++) {
//        printf("%c ", sourceText[i]);
//    }
//    printf("\n");

    // 与pattern做比较
    int sourceTextLength = (int)sourceText.length();
    int lastSearchPos = sourceTextLength - patternLength;

    if (pattern->ignoreCase()) {
        // 忽视大小写
        std::string upper = pattern->upperCasePattern();
        char firstCharLower = lower[0];
        char firstCharUpper = upper[0];

        for (int i = 0; i <= lastSearchPos; i++) {
            int indexInTotal = searchStartPos + i;

            char current = sourceText[i];
//            printf("i: %d, current: %c\n", i, current);
            if (current == firstCharLower || current == firstCharUpper) {
                int j = 1;
                int k = i + 1;
                for (; j < patternLength; k++) {
                    char lowerJ = lower[j];
                    char upperJ = upper[j];
                    char symbol = sourceText[k];
//                    printf("j: %d, lower[j]: %c, upper[j]: %c\n", j, lowerJ, upperJ);
//                    printf("k: %d, symbol: %c\n", k, symbol);
                    if (std::strcmp(&symbol, "\u200b") == 0) {
                        if (patternLength - j > sourceTextLength - k) {
                            // sourceText剩余字符数量小于pattern的剩余字符数量，无法继续搜索，跳出循环
//                            printf("patternLength - j > sourceTextLength - k\n");
                            break;
                        } else {
                            // sourceText剩余字符数量大于pattern的剩余字符数量，跳过该次循环，继续下次搜索
//                            printf("patternLength - j <= sourceTextLength - k\n");
                            continue;
                        }
                    }
                    if (lowerJ != symbol && upperJ != symbol) {
                        // 文字不匹配，跳出循环
//                        printf("lowerJ != symbol && upperJ != symbol\n");
                        break;
                    }
                    ++j;
                }

                if (j == patternLength) {
                    // 搜索到pattern的最后一个字符，可确定搜索成功，返回搜索结果
                    int resultStart = indexInTotal - offset;
                    int resultLength = k - i;
//                    std::string resultStr = "";
//                    resultStr.append(&sourceText[i], resultLength);
//                    printf("find result at start: %d, length: %d, text: %s\n", resultStart, resultLength, resultStr.c_str());

                    return new Result(resultStart, resultLength);
                }
            }
        }

    } else {
        // 大小写完全匹配
        char firstChar = lower[0];

        for (int i = 0; i <= lastSearchPos; i++) {
            int indexInTotal = searchStartPos + i;

            char current = sourceText[i];
            if (current == firstChar) {
                int j = 1;
                int k = i + 1;
                for (; j < patternLength; k++) {
                    char lowerJ = lower[j];
                    char symbol = sourceText[k];
                    if (std::strcmp(&symbol, "\u200b") == 0) {
                        if (patternLength - j > sourceTextLength - k) {
                            // sourceText剩余字符数量小于pattern的剩余字符数量，无法继续搜索，跳出循环
//                            printf("patternLength - j > sourceTextLength - k\n");
                            break;
                        } else {
                            // sourceText剩余字符数量大于pattern的剩余字符数量，跳过该次循环，继续下次搜索
//                            printf("patternLength - j <= sourceTextLength - k\n");
                            continue;
                        }
                    }
                    if (lowerJ != symbol) {
                        // 文字不匹配，跳出循环
//                        printf("lowerJ != symbol\n");
                        break;
                    }
                    ++j;
                }

                if (j >= patternLength) {
                    // 搜索到pattern的最后一个字符，可确定搜索成功，返回搜索结果
                    int resultStart = indexInTotal - offset;
                    int resultLength = k - i;
//                    std::string resultStr = "";
//                    resultStr.append(&sourceText[i], resultLength);
//                    printf("find result at start: %d, length: %d, text: %s\n", resultStart, resultLength, resultStr.c_str());

                    return new Result(resultStart, resultLength);
                }
            }
        }
    }
    return NULL;
}
