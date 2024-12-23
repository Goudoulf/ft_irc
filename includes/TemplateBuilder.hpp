#ifndef TEMPLATEBUILDER_HPP
#define TEMPLATEBUILDER_HPP

#include "Command.hpp"
#include "CommandParser.hpp"
#include "ParamTemplate.hpp"

#include "CmdLevel.h"

#include <string>
#include <map>

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
    std::vector<std::pair<std::string, const ParamTemplate *> > _params;
  };

protected:
        
        TemplateBuilder(const std::string &name, CmdLevel level,
        const std::vector<std::pair<std::string, const ParamTemplate *> > &_params,
        Command *command, CommandParser *parser);

private:
  friend class CommandDirector;
  std::string     _name;
  CmdLevel        _levelNeeded;
  CommandParser *_parser;
  std::map<std::string, std::vector<std::string> > _parsedParams;
  std::vector<std::pair<std::string, const ParamTemplate *> > _params;
  Command *_command;
  bool    checkLevel(Client *client)const;
  bool    fillParam(Client *client, std::vector<std::vector<std::string> > params);
  void    executeCommand(Client *client, const std::string &param);
};

#endif
