/*
 * libfscp - C++ portable OpenSSL cryptographic wrapper library.
 * Copyright (C) 2010-2011 Julien Kauffmann <julien.kauffmann@freelan.org>
 *
 * This file is part of libfscp.
 *
 * libfscp is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libfscp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use libfscp in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file clear_session_message.hpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief A session message class.
 */

#ifndef FSCP_CLEAR_SESSION_MESSAGE_HPP
#define FSCP_CLEAR_SESSION_MESSAGE_HPP

#include "buffer_tools.hpp"

#include <stdint.h>
#include <cstring>

#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace fscp
{
	/**
	 * \brief A clear session message class.
	 */
	class clear_session_message
	{
		public:

			/**
			 * \brief The session number type.
			 */
			typedef uint32_t session_number_type;

			/**
			 * \brief The key length.
			 */
			static const size_t KEY_LENGTH = 32;

			/**
			 * \brief The iv length.
			 */
			static const size_t IV_LENGTH = 16;

			/**
			 * \brief The key type.
			 */
			typedef boost::array<uint8_t, KEY_LENGTH> key_type;

			/**
			 * \brief The iv type.
			 */
			typedef boost::array<uint8_t, IV_LENGTH> iv_type;

			/**
			 * \brief Write a session message to a buffer.
			 * \param buf The buffer to write to.
			 * \param buf_len The length of buf.
			 * \param session_number The session number.
			 * \param sig_key The signature key.
			 * \param enc_key The encryption key.
			 * \param iv The initialization vector.
			 * \return The count of bytes written.
			 */
			static size_t write(void* buf, size_t buf_len, session_number_type session_number, key_type sig_key, key_type enc_key, iv_type iv);

			/**
			 * \brief Create a clear_session_message and map it on a buffer.
			 * \param buf The buffer.
			 * \param buf_len The buffer length.
			 *
			 * If the mapping fails, a std::runtime_error is thrown.
			 */
			clear_session_message(const void* buf, size_t buf_len);

			/**
			 * \brief Get the session number.
			 * \return The session number.
			 */
			session_number_type session_number() const;

			/**
			 * \brief Get the signature key.
			 * \return The signature key.
			 */
			const uint8_t* signature_key() const;

			/**
			 * \brief Get the signature key size.
			 * \return The signature key size.
			 */
			size_t signature_key_size() const;

			/**
			 * \brief Get the encryption key.
			 * \return The encryption key.
			 */
			const uint8_t* encryption_key() const;

			/**
			 * \brief Get the encryption key size.
			 * \return The encryption key size.
			 */
			size_t encryption_key_size() const;

			/**
			 * \brief Get the initialization vector.
			 * \return The initialization vector.
			 */
			const uint8_t* initialization_vector() const;

			/**
			 * \brief Get the initialization vector size.
			 * \return The initialization vector size.
			 */
			size_t initialization_vector_size() const;

		protected:

			/**
			 * \brief The length of the body.
			 */
			static const size_t BODY_LENGTH = 16;

			/**
			 * \brief The data.
			 * \return The data buffer.
			 */
			const uint8_t* data() const;

		private:

			const void* m_data;
	};

	inline clear_session_message::session_number_type clear_session_message::session_number() const
	{
		return ntohl(buffer_tools::get<session_number_type>(data(), 0));
	}

	inline const uint8_t* clear_session_message::signature_key() const
	{
		return data() + sizeof(session_number_type) + sizeof(uint16_t);
	}

	inline size_t clear_session_message::signature_key_size() const
	{
		return ntohs(buffer_tools::get<uint16_t>(data(), sizeof(session_number_type)));
	}

	inline const uint8_t* clear_session_message::encryption_key() const
	{
		return data() + sizeof(session_number_type) + sizeof(uint16_t) + signature_key_size() + sizeof(uint16_t);
	}

	inline size_t clear_session_message::encryption_key_size() const
	{
		return ntohs(buffer_tools::get<uint16_t>(data(), sizeof(session_number_type) + sizeof(uint16_t) + signature_key_size()));
	}

	inline const uint8_t* clear_session_message::initialization_vector() const
	{
		return data() + sizeof(session_number_type) + sizeof(uint16_t) + signature_key_size() + sizeof(uint16_t) + encryption_key_size() + sizeof(uint16_t);
	}

	inline size_t clear_session_message::initialization_vector_size() const
	{
		return ntohs(buffer_tools::get<uint16_t>(data(), sizeof(session_number_type) + sizeof(uint16_t) + signature_key_size() + sizeof(uint16_t) + encryption_key_size()));
	}

	inline const uint8_t* clear_session_message::data() const
	{
		return static_cast<const uint8_t*>(m_data);
	}
}

#endif /* FSCP_CLEAR_SESSION_MESSAGE_HPP */
