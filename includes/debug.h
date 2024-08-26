/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassie <cassie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:34:54 by cassie            #+#    #+#             */
/*   Updated: 2024/08/26 08:36:59 by cassie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
#define DEBUG_H
#include <string>

enum LogLevel { DEBUG, INFO, CMD, REPLY, WARN , ERROR};
extern LogLevel currentLogLevel;

void log(LogLevel level, const std::string message);

#endif
