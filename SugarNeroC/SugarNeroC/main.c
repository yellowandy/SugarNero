//
//  main.c
//  SugarNeroC
//
//  Created by Yuk Lai Suen on 4/20/15.
//  Copyright (c) 2015 Yuk Lai. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "SugarNeroHelper.h"

#define FAIL printf("Failed at line %d\n", __LINE__)

void testGetSugarBuildMark() {
    SUGAR_BUILD_MARK buildMark;
    SUGAR_BUILD_RESULT buildResult;
    MatchCondition matchedConditions[5] = {0};
    size_t conditionCount = 0;
    
    buildResult = getSugarBuildMark("Just another line of php", &buildMark, matchedConditions, ARRAY_SIZE(matchedConditions), &conditionCount);
    if (SUGAR_BUILD_RESULT_PASS != buildResult) { FAIL; }
    if (SUGAR_BUILD_MARK_NONE != buildMark) { FAIL; }
    
    buildResult = getSugarBuildMark("   // BEGIN SUGARCRM flav=pro ONLY", &buildMark, matchedConditions, ARRAY_SIZE(matchedConditions), &conditionCount);
    if (SUGAR_BUILD_RESULT_PASS != buildResult) { FAIL; }
    if (SUGAR_BUILD_MARK_BEGIN != buildMark) { FAIL; }
    
    buildResult = getSugarBuildMark("// END SUGARCRM flav=pro ONLY", &buildMark, matchedConditions, ARRAY_SIZE(matchedConditions), &conditionCount);
    if (SUGAR_BUILD_RESULT_PASS != buildResult) { FAIL; }
    if (SUGAR_BUILD_MARK_END != buildMark) { FAIL; }
    
    buildResult = getSugarBuildMark("// FILE SUGARCRM flav=pro ONLY", &buildMark, matchedConditions, ARRAY_SIZE(matchedConditions), &conditionCount);
    if (SUGAR_BUILD_RESULT_PASS != buildResult) { FAIL; }
    if (SUGAR_BUILD_MARK_FILE != buildMark) { FAIL; }
}

void testParseSugarBuildOptions() {
    // we have seen at most two so far, at most, e.g. "flav=pro && ent=os" are two matched conditions
    MatchCondition matchedConditions[5] = {0};
    size_t conditionCount = 0;
    
    parseSugarBuildOptions("flav=ent && dep!=od", matchedConditions, ARRAY_SIZE(matchedConditions), &conditionCount);
    if (0 != strncmp(matchedConditions[0].logicalOperator, "", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[0].key, "flav", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[0].operator, "=", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[0].value, "ent", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[1].logicalOperator, "&&", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[1].key, "dep", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[1].operator, "=", 0)) { FAIL; }
    if (0 != strncmp(matchedConditions[1].value, "od", 0)) { FAIL; }
    if (2 != conditionCount) { FAIL; }
}

void testLineCountInString() {
    if (1 != lineCountInString("")) { FAIL; }
    if (1 != lineCountInString(" ")) { FAIL; }
    if (2 != lineCountInString(" \n ")) { FAIL; }
    if (3 != lineCountInString(" \n\n ")) { FAIL; }
    if (3 != lineCountInString(" \n \n ")) { FAIL; }
}

void testStringToArrayOfLines() {
    char **array;
    size_t count;
    
    stringToArrayOfLines("", &array, &count);
    if (1 != count) { FAIL; }
    if (0 != strcmp(array[0], "")) { FAIL; }
    free(array);
    
    stringToArrayOfLines("\n", &array, &count);
    if (2 != count) { FAIL; }
    if (0 != strcmp(array[0], "")) { FAIL; }
    if (0 != strcmp(array[1], "")) { FAIL; }
    free(array);

    stringToArrayOfLines("blah", &array, &count);
    if (1 != count) { FAIL; }
    if (0 != strcmp(array[0], "blah")) { FAIL; }
    free(array);
    
    stringToArrayOfLines("blah\nfoo\n", &array, &count);
    if (3 != count) { FAIL; }
    if (0 != strcmp(array[0], "blah")) { FAIL; }
    if (0 != strcmp(array[1], "foo")) { FAIL; }
    if (0 != strcmp(array[2], "")) { FAIL; }
    free(array);
}

void testArrayOfLinesToString() {
    char *array0[] = {""};
    char *array1[] = {"", ""};
    char *array2[] = {"foo", "blah"};
    char *array3[] = {"foo1", "foo2", "", "foo3"};
    
    size_t count0 = ARRAY_SIZE(array0);
    size_t count1 = ARRAY_SIZE(array1);
    size_t count2 = ARRAY_SIZE(array2);
    size_t count3 = ARRAY_SIZE(array3);
    
    const char *result;
    
    result = arrayOfLinesToString(array0, count0);
    if (0 != strcmp("", result)) { FAIL; }
    
    result = arrayOfLinesToString(array1, count1);
    if (0 != strcmp("\n", result)) { FAIL; }
    
    result = arrayOfLinesToString(array2, count2);
    if (0 != strcmp("foo\nblah", result)) { FAIL; }
    
    result = arrayOfLinesToString(array3, count3);
    if (0 != strcmp("foo1\nfoo2\n\nfoo3", result)) { FAIL; }
}

void testCommentOutLine() {
    char *line;
    
    line = commentOutLine("");
    if (0 != strcmp("// ", line)) { FAIL; }
    
    line = commentOutLine("hello world");
    if (0 != strcmp("// hello world", line)) { FAIL; }
    free(line);
}

void testMatchesCondition() {
    MatchCondition condition0 = {"", "flav", "=", "pro"};
    MatchCondition condition1 = {"", "flav", "!=", "pro"};
    
    if (matchesCondition(condition0, SUGAR_BUILD_FLAVOR_ENTERPRISE)) { FAIL; }
    if (!matchesCondition(condition0, SUGAR_BUILD_FLAVOR_PROFESSIONAL)) { FAIL; }
    if (!matchesCondition(condition1, SUGAR_BUILD_FLAVOR_ENTERPRISE)) { FAIL; }
}

void testMatchesConditions() {
    MatchCondition condition0 = {"", "flav", "=", "pro"};
    MatchCondition condition1 = {"&&", "flav", "!=", "ult"};
    MatchCondition condition2 = {"||", "flav", "!=", "ult"};
    MatchCondition conditionsA[] = {condition0, condition1};
    MatchCondition conditionsB[] = {condition0, condition2};
    
    if (matchesConditions(conditionsA, ARRAY_SIZE(conditionsA), SUGAR_BUILD_FLAVOR_ENTERPRISE)) { FAIL; }
    if (!matchesConditions(conditionsA, ARRAY_SIZE(conditionsA), SUGAR_BUILD_FLAVOR_PROFESSIONAL)) { FAIL; }
    
    if (!matchesConditions(conditionsB, ARRAY_SIZE(conditionsB), SUGAR_BUILD_FLAVOR_PROFESSIONAL)) { FAIL; }
    if (!matchesConditions(conditionsB, ARRAY_SIZE(conditionsB), SUGAR_BUILD_FLAVOR_ENTERPRISE)) { FAIL; }
    if (matchesConditions(conditionsB, ARRAY_SIZE(conditionsB), SUGAR_BUILD_FLAVOR_ULTIMATE)) { FAIL; }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Start to parse file\n");
    const char *filePath = "/Users/ysuen/Documents/SugarNeroC/SugarNeroC/EditView.php";
    
    char *buffer;
    size_t size;
    copyFileContentToBuffer(filePath, &buffer, &size);
    
    processFile(buffer, SUGAR_BUILD_FLAVOR_ULTIMATE);
    printf("End parse file\n");
    
    testLineCountInString();
    testStringToArrayOfLines();
    testArrayOfLinesToString();
    testGetSugarBuildMark();
    testParseSugarBuildOptions();
    testCommentOutLine();
    testMatchesCondition();

    return 0;
}

