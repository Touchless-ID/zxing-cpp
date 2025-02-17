/*
* Copyright 2016 Nu-book Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "JNIUtils.h"

#include <stdexcept>
#include <vector>

static bool RequiresSurrogates(uint32_t ucs4)
{
	return ucs4 >= 0x10000;
}

static uint16_t HighSurrogate(uint32_t ucs4)
{
	return uint16_t((ucs4 >> 10) + 0xd7c0);
}

static uint16_t LowSurrogate(uint32_t ucs4)
{
	return uint16_t(ucs4 % 0x400 + 0xdc00);
}

static void Utf32toUtf16(const uint32_t* utf32, size_t length, std::vector<uint16_t>& result)
{
	result.clear();
	result.reserve(length);
	for (size_t i = 0; i < length; ++i)	{
		uint32_t c = utf32[i];
		if (RequiresSurrogates(c)) {
			result.push_back(HighSurrogate(c));
			result.push_back(LowSurrogate(c));
		} else {
			result.push_back(c);
		}
	}
}

jstring C2JString(JNIEnv* env, const std::wstring& str)
{
	if (env->ExceptionCheck())
		return 0;

	if constexpr (sizeof(wchar_t) == 2) {
		return env->NewString((const jchar*)str.data(), str.size());
	} else {
		std::vector<uint16_t> buffer;
		Utf32toUtf16((const uint32_t*)str.data(), str.size(), buffer);
		return env->NewString((const jchar*)buffer.data(), buffer.size());
	}
}

jstring C2JString(JNIEnv* env, const std::string& str)
{
	return C2JString(env, std::wstring(str.begin(), str.end()));
}

std::string J2CString(JNIEnv* env, jstring str)
{
	const jsize len = env->GetStringLength(str);
	std::string res(len, 0);

	// Translates 'len' number of Unicode characters into modified
	// UTF-8 encoding and place the result in the given buffer.
	env->GetStringUTFRegion(str, 0, len, res.data());

	return res;
}
