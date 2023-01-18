#include "template.h"

TemplateInfo::TemplateInfo(string name, string author, TemplatePlatform *platform)
	: _name(&name), _author(&author), _platform(*platform) {}

TemplateProject::TemplateProject()
{}

Template::Template(TemplateInfo info, TemplateProject project)
	: _info(&info), _project(&project) {}

TemplateInfo Template::info()
{
	return *_info;
}

TemplateProject Template::project()
{
	return *_project;
}