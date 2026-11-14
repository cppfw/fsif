/*
The MIT License (MIT)

Copyright (c) 2015-2025 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* ================ LICENSE END ================ */

#pragma once

#include <memory>

#include <utki/debug.hpp>
#include <utki/unique_ref.hpp>

#include "file.hpp"

namespace fsif {

/**
 * @brief A file located inside a zip archive.
 * A zip_file is a file object which accesses the data of a particular file
 * located inside a zip archive.
 */
class zip_file : public fsif::file
{
	utki::unique_ref<fsif::file> underlying_zip_file;

	void* handle = nullptr;

public:
	/**
	 * @brief Constructor.
	 * @param underlying_zip_file - the underlying zip archive file.
	 * @param path - path to the file inside the zip archive.
	 */
	zip_file(
		utki::unique_ref<fsif::file> underlying_zip_file, //
		std::string_view path = std::string_view()
	);

	zip_file(const zip_file&) = delete;
	zip_file& operator=(const zip_file&) = delete;

	zip_file(zip_file&&) = delete;
	zip_file& operator=(zip_file&&) = delete;

	/**
	 * @brief Destructor.
	 */
	~zip_file() noexcept override;

	/**
	 * @brief Open the file.
	 * @param mode - file open mode.
	 */
	void open_internal(fsif::mode mode) override;

	/**
	 * @brief Close the file.
	 */
	void close_internal() const noexcept override;

	/**
	 * @brief Read data from the file.
	 * @param buf - buffer to read data into.
	 * @return number of bytes actually read.
	 */
	size_t read_internal(utki::span<uint8_t> buf) const override;

	/**
	 * @brief Check if the file exists.
	 * @return true if the file exists.
	 */
	bool exists() const override;

	/**
	 * @brief List directory entries.
	 * @param max_entries - maximum number of entries to return.
	 * @return list of directory entries.
	 */
	std::vector<std::string> list_dir(size_t max_entries = 0) const override;

	/**
	 * @brief Spawn a new file object.
	 * @return a new file object which can be used to access the same file
	 * concurrently with this one.
	 */
	utki::unique_ref<fsif::file> spawn() override
	{
		return utki::make_unique<zip_file>( //
			this->underlying_zip_file.get().spawn( //
				this->underlying_zip_file.get().path()
			)
		);
	}
};

} // namespace fsif
