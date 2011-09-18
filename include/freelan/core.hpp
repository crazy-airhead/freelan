/*
 * libfreelan - A C++ library to establish peer-to-peer virtual private
 * networks.
 * Copyright (C) 2010-2011 Julien KAUFFMANN <julien.kauffmann@freelan.org>
 *
 * This file is part of libfreelan.
 *
 * libfreelan is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libfreelan is distributed in the hope that it will be useful, but
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
 * If you intend to use libfreelan in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file core.hpp
 * \author Julien KAUFFMANN <julien.kauffmann@freelan.org>
 * \brief The freelan core class.
 */

#ifndef FREELAN_CORE_HPP
#define FREELAN_CORE_HPP

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>

#include <asiotap/asiotap.hpp>
#include <fscp/fscp.hpp>

#include "configuration.hpp"

namespace freelan
{
	/**
	 * \brief The core class.
	 */
	class core
	{
		public:

			/**
			 * \brief The endpoint type.
			 */
			typedef fscp::server::ep_type ep_type;

			/**
			 * \brief The certificate type.
			 */
			typedef fscp::server::cert_type cert_type;

			/**
			 * \brief The identity store type.
			 */
			typedef fscp::identity_store identity_store;

			/**
			 * \brief The contact period.
			 */
			static const boost::posix_time::time_duration CONTACT_PERIOD;

			/**
			 * \brief A session established callback.
			 * \param host The host with which a session is established.
			 */
			typedef boost::function<void (const ep_type& host)> session_established_callback;

			/**
			 * \brief A session lost callback.
			 * \param host The host with which a session was lost.
			 */
			typedef boost::function<void (const ep_type& host)> session_lost_callback;

			/**
			 * \brief The constructor.
			 * \param io_service The io_service to bind to.
			 * \param configuration The configuration to use.
			 */
			core(boost::asio::io_service& io_service, const freelan::configuration& configuration);

			/**
			 * \brief Get the configuration.
			 * \return The current configuration.
			 */
			const freelan::configuration& configuration() const;

			/**
			 * \brief Get the associated tap adapter.
			 * \return The associated tap adapter.
			 */
			const asiotap::tap_adapter& tap_adapter() const;

			/**
			 * \brief Get the associated server.
			 * \return The associated server.
			 */
			const fscp::server& server() const;

			/**
			 * \brief Set the session established callback.
			 * \param callback The callback.
			 */
			void set_session_established_callback(session_established_callback callback);

			/**
			 * \brief Set the session lost callback.
			 * \param callback The callback.
			 */
			void set_session_lost_callback(session_lost_callback callback);

			/**
			 * \brief Open the current core instance.
			 */
			void open();

			/**
			 * \brief Close the current core instance.
			 */
			void close();

		private:

			//fscp::server related methods
			void async_greet(const ep_type&);
			bool on_hello_request(fscp::server&, const ep_type&, bool);
			void on_hello_response(fscp::server&, const ep_type&, const boost::posix_time::time_duration&, bool);
			bool on_presentation(fscp::server&, const ep_type&, cert_type, cert_type, bool);
			bool on_session_request(fscp::server&, const ep_type&, bool);
			void on_session_established(fscp::server&, const ep_type&);
			void on_session_lost(fscp::server&, const ep_type&);
			void on_data(fscp::server&, const ep_type&, boost::asio::const_buffer);

			//asiotap::tap_adapter related methods
			void tap_adapter_read_done(asiotap::tap_adapter&, const boost::system::error_code&, size_t);

			//other methods
			void do_contact();
			void do_contact(const boost::system::error_code&);

			freelan::configuration m_configuration;
			fscp::server m_server;
			asiotap::tap_adapter m_tap_adapter;
			boost::array<unsigned char, 65536> m_tap_adapter_buffer;
			boost::asio::deadline_timer m_contact_timer;

			//callbacks
			session_established_callback m_session_established_callback;
			session_lost_callback m_session_lost_callback;
	};

	inline const freelan::configuration& core::configuration() const
	{
		return m_configuration;
	}
	
	inline const asiotap::tap_adapter& core::tap_adapter() const
	{
		return m_tap_adapter;
	}
	
	inline const fscp::server& core::server() const
	{
		return m_server;
	}

	inline void core::set_session_established_callback(session_established_callback callback)
	{
		m_session_established_callback = callback;
	}

	inline void core::set_session_lost_callback(session_lost_callback callback)
	{
		m_session_lost_callback = callback;
	}
}

#endif /* FREELAN_CORE_HPP */
