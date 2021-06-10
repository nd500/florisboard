/*
 * Copyright (C) 2021 Patrick Goldinger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include "ime/spelling//spellingdict.h"
#include "utils/jni_utils.h"

#pragma ide diagnostic ignored "UnusedLocalVariable"

using namespace ime::spellcheck;

extern "C"
JNIEXPORT jlong JNICALL
Java_dev_patrickgold_florisboard_ime_spelling_SpellingDict_00024Companion_nativeInitialize(
        JNIEnv *env,
        jobject thiz,
        jstring base_path,
        jstring aff_file_name,
        jstring dic_file_name) {
    auto strBasePath = utils::j2std_string(env, base_path);
    auto strAffFileName = utils::j2std_string(env, aff_file_name);
    auto strDicFileName = utils::j2std_string(env, dic_file_name);

    std::string aff = strBasePath + "/" + strAffFileName;
    std::string dic = strBasePath + "/" + strDicFileName;
    auto *spellingDict = new SpellingDict(aff, dic);

    return reinterpret_cast<jlong>(spellingDict);
}

extern "C"
JNIEXPORT void JNICALL
Java_dev_patrickgold_florisboard_ime_spelling_SpellingDict_00024Companion_nativeDispose(
        JNIEnv *env,
        jobject thiz,
        jlong native_ptr) {
    auto spellingDict = reinterpret_cast<SpellingDict *>(native_ptr);

    delete spellingDict;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_dev_patrickgold_florisboard_ime_spelling_SpellingDict_00024Companion_nativeSpell(
        JNIEnv *env,
        jobject thiz,
        jlong native_ptr,
        jstring word) {
    auto strWord = utils::j2std_string(env, word);

    auto spellingDict = reinterpret_cast<SpellingDict *>(native_ptr);
    auto result = spellingDict->spell(strWord);

    return result;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_dev_patrickgold_florisboard_ime_spelling_SpellingDict_00024Companion_nativeSuggest(
        JNIEnv *env,
        jobject thiz,
        jlong native_ptr,
        jstring word) {
    auto strWord = utils::j2std_string(env, word);

    auto spellingDict = reinterpret_cast<SpellingDict *>(native_ptr);
    auto result = spellingDict->suggest(strWord);

    jclass jStringClass = env->FindClass("java/lang/String");
    jobjectArray jSuggestions = env->NewObjectArray(result.size(), jStringClass, nullptr);
    for (int n = 0; n < result.size(); n++) {
        env->SetObjectArrayElement(jSuggestions, n, utils::std2j_string(env, result[n]));
    }

    return jSuggestions;
}