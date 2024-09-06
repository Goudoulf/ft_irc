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

#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>
#include <unordered_map>

class Reply
{
	public:
			
			Reply();
			~Reply();
			void sendIRCReply(int code, const std::unordered_map<std::string, std::string>& params);
		
	private:
		
		Reply(Reply& copy);
		Reply& operator=(Reply& copy);
		std::unordered_map<int, std::string> _ReplyTemplates;
		std::unordered_map<int, std::string> _ErrorTemplates;
};

#endif
