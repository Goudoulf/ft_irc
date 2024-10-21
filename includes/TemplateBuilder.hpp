#pragma once
#include "Command.hpp"
#include "CommandParser.hpp"
#include "ParamTemplate.hpp"
#include "cmds.h"
#include "reply.h"
#include <string>
#include <utility>
#include <map>
#include "CmdLevel.h"

class Command;
class CommandParser;
class ParamTemplate;

class TemplateBuilder {

public:
  ~TemplateBuilder();
  const std::string getName() const;

  class Builder {

  public:
    Builder();
    Builder &name(std::string name);
    Builder &level(CmdLevel level);
    Builder &param(std::string type, const ParamTemplate *checker);
    Builder &trailing(std::string param, const ParamTemplate *checker);
    Builder &parser(CommandParser *parser);
    Builder &command(Command *command);

    TemplateBuilder *build();

  private:
    std::string     _name;
    CmdLevel        _level;
    Command         *_command;
    CommandParser *_parser;
    std::vector<std::pair<std::string, const ParamTemplate *>> _params;
  };

protected:
        
        TemplateBuilder(const std::string &name, CmdLevel level,
        const std::vector<std::pair<std::string, const ParamTemplate *>> &_params,
        Command *command, CommandParser *parser);

private:
  friend class CommandDirector;
  std::string     _name;
  CmdLevel        _levelNeeded;
  std::map<std::string, std::vector<std::string>> _parsedParams;
  CommandParser *_parser;
  std::vector<std::pair<std::string, const ParamTemplate *>> _params;
  Command *_command;
  bool    check_level(Client *client)const;
  bool    fill_param(Client *client, std::vector<std::vector<std::string>> params);
  void    executeCommand(Client *client, const std::string &param);
};
