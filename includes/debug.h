/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvallini <lvallini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 08:34:54 by cassie            #+#    #+#             */
/*   Updated: 2024/10/31 13:30:09 by lvallini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
#define DEBUG_H

enum LogLevel { DEBUG, INFO, CMD, REPLY, WARN , ERROR};
extern LogLevel currentLogLevel;

void log(LogLevel level, const std::string message);

#endif
