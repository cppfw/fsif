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

#include <utki/config.hpp>
#include <utki/unique_ref.hpp>

#include "file.hpp"

namespace fsif {

// TODO: doxygen
class root_dir : public file
{
	union base_file_union {
		utki::unique_ref<file> variable;
		utki::unique_ref<const file> constant;

		base_file_union(utki::unique_ref<file> f) :
			variable(std::move(f))
		{}

		base_file_union(utki::unique_ref<const file> f) :
			constant(std::move(f))
		{}

		base_file_union(const base_file_union&) = delete;
		base_file_union& operator=(const base_file_union&) = delete;

		base_file_union(base_file_union&&) = delete;
		base_file_union& operator=(base_file_union&&) = delete;

		~base_file_union()
		{
			// doesn't matter on which memeber to call the destrcutor, as the
			// difference is only in constness
			this->variable.~unique_ref();
		}
	} base_file;

	std::string root_directory;

	// This constructor is private because the object it constructs can only be
	// used in const context.
	root_dir(
		utki::unique_ref<const file> base_file, //
		std::string root_directory,
		bool // dummy parameter to disambiguate from the another constructor
	) :
		base_file(std::move(base_file)),
		root_directory(std::move(root_directory))
	{
		this->init();
	}

	void init()
	{
		this->file::set_path_internal(std::string(this->base_file.constant.get().path()));
		this->base_file.variable.get().set_path(this->root_directory + this->path());
	}

public:
	/**
	 * @param base_file - a file to wrap.
	 * @param root_directory - path to the root directory to set. It should have
	 * trailing '/' character.
	 */
	root_dir(
		utki::unique_ref<file> base_file, //
		std::string root_directory
	) :
		base_file(std::move(base_file)),
		root_directory(std::move(root_directory))
	{
		this->init();
	}

	/**
	 * @brief Make root_dir for a constant base file.
	 * @param base_file - constant base file.
	 * @param root_directory - path to the root directory to set. It should have
	 * trailing '/' character.
	 * @return constant root_dir.
	 */
	static utki::unique_ref<const root_dir> make(utki::unique_ref<const file> base_file, std::string root_directory)
	{
		return utki::unique_ref(std::unique_ptr<const root_dir>(new root_dir(
			std::move(base_file),
			std::move(root_directory),
			false // dummy parameter to disambiguate from the another constructor
		)));
	}

	root_dir(const root_dir&) = delete;
	root_dir& operator=(const root_dir&) = delete;

	root_dir(root_dir&&) = delete;
	root_dir& operator=(root_dir&&) = delete;

	~root_dir() override = default;

private:
	void set_path_internal(std::string path_name) const override
	{
		this->file::set_path_internal(std::move(path_name));
		this->base_file.constant.get().set_path(this->root_directory + this->path());
	}

	void open_internal(fsif::mode io_mode) override
	{
		this->base_file.variable.get().open(io_mode);
	}

	void close_internal() const noexcept override
	{
		this->base_file.constant.get().close();
	}

	std::vector<std::string> list_dir(size_t max_entries = 0) const override
	{
		return this->base_file.constant.get().list_dir(max_entries);
	}

	size_t read_internal(utki::span<uint8_t> buf) const override
	{
		return this->base_file.constant.get().read(buf);
	}

	size_t write_internal(utki::span<const uint8_t> buf) override
	{
		return this->base_file.variable.get().write(buf);
	}

	size_t seek_forward_internal(size_t num_bytes_to_seek) const override
	{
		return this->base_file.constant.get().seek_forward(num_bytes_to_seek);
	}

	size_t seek_backward_internal(size_t num_bytes_to_seek) const override
	{
		return this->base_file.constant.get().seek_backward(num_bytes_to_seek);
	}

	void rewind_internal() const override
	{
		this->base_file.constant.get().rewind();
	}

	void make_dir() override
	{
		this->base_file.variable.get().make_dir();
	}

	bool exists() const override
	{
		return this->base_file.constant.get().exists();
	}

	utki::unique_ref<file> spawn() override
	{
		return utki::make_unique<root_dir>(
			this->base_file.variable.get().spawn(), //
			this->root_directory
		);
	}
};

} // namespace fsif
