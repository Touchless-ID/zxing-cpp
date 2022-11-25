#pragma once
/*
* Copyright 2019 Axel Waggershauser
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

#include "DecodeHints.h"
#include "ImageView.h"
#include "Result.h"

#include <memory>

namespace ZXing {

class BinaryBitmap;

class LumImage : public ImageView
{
	std::unique_ptr<uint8_t[]> _memory;
	LumImage(std::unique_ptr<uint8_t[]>&& data, int w, int h)
		: ImageView(data.get(), w, h, ImageFormat::Lum), _memory(std::move(data))
	{}

public:
	LumImage() : ImageView(nullptr, 0, 0, ImageFormat::Lum) {}
	LumImage(int w, int h) : LumImage(std::make_unique<uint8_t[]>(w * h), w, h) {}

	uint8_t* data() { return _memory.get(); }
};

/**
 * Read barcode from an ImageView
 *
 * @param buffer  view of the image data including layout and format
 * @param hints  optional DecodeHints to parameterize / speed up decoding
 * @return #Result structure
 */
Result ReadBarcode(const ImageView& buffer, const DecodeHints& hints = {});

// WARNING: this API is experimental and may change/disappear
Results ReadBarcodes(const ImageView& buffer, const DecodeHints& hints = {});

std::pair<std::unique_ptr<BinaryBitmap>, std::unique_ptr<LumImage>>
GetBinarizedBitmap(const ImageView& buffer, const DecodeHints& hints);

} // ZXing

