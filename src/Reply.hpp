/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:10:42 by cassie            #+#    #+#             */
/*   Updated: 2024/09/06 10:16:28 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
// #ifndef REPLY_HPP
// #define REPLY_HPP

#include <string>
#include <map>

class Reply
{
	public:
			
			Reply();
			~Reply();
			void sendIRCReply(std::string code, const std::map<std::string, std::string>& params);
		
	private:
		
		Reply(Reply& copy);
		Reply& operator=(Reply& copy);
		std::map<std::string, std::string> _ReplyTemplates;
		std::map<std::string, std::string> _ErrorTemplates;
};

// #endif
