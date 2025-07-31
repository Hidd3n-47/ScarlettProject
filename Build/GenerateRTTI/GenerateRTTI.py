import os
import re
from typing import Optional, Match

ROOT_SEARCH_DIR             = "E:/Programming/ScarlettProject/ScarlEntt/ScarlEntt/Components/"
GENERATED_RTTI_OUTPUT_DIR   = ROOT_SEARCH_DIR + "Generated/"

COMPONENT_MACRO = "SCARLETT_COMPONENT("

types = {
     "float"                : "FLOAT"   ,
     "std::string"          : "STRING"  ,
     "ScarlettMath::Vec3"   : "VEC3"    ,
     "ScarlettMath::Vec4"   : "VEC4"    ,
     "ScarlettMath::Quat"   : "QUAT"
}

# Function used to find all files that are considered "Components"
# These are files that have the C++ macro 'SCARLETT_COMPONENT()'
def get_component_files(path):
    result = []
    for root, dirs, files in os.walk(path):
        for file in files:
            filepath = os.path.join(root,file)

            if not str(filepath).endswith(".h"):
                continue

            component_found = False
            with open(filepath, "r") as f:
                content = f.read()
                component_lines = content.splitlines()
                for line in component_lines:
                    if line.find(COMPONENT_MACRO) != -1:
                        component_found = True

            if component_found:
                result.append(component_lines)
    return result

def generate_rtti_for_component(struct_name, member_variables):
    os.makedirs(GENERATED_RTTI_OUTPUT_DIR, exist_ok=True)
    output_path = os.path.join(GENERATED_RTTI_OUTPUT_DIR, struct_name + ".generated.cpp")

    property_string = ""
    for variable in member_variables:
        property_string += "\n    { \"" + variable[1] + "\", Property{} },"

    with open(output_path, "w+") as f:
        f.write('''\
#include "ScarlEnttpch.h"
#include "Components/{struct_name}.h"

#include "RTTI/TypeReflection.h"
#include "ScarlEntt/ComponentManager.h"

namespace Scarlett::Component
{{

#ifdef DEV_CONFIGURATION

void {struct_name}::GenerateProperties()
{{
    mProperties.clear();
'''.format(struct_name=struct_name))

        for variable in member_variables:
            if variable[0] in types:
                f.write('''\

    mProperties["{property_name}"] = ScarlEntt::Property {{
        ScarlEntt::PropertyType::{type_enum},
        ScarlEntt::ComponentManager::GetComponentTypeId<{struct_name}>(),
        [this]() {{ return ScarlEntt::TypeReflection::GetStringFromValue(this->{property_name}); }},
        [this](const std::string_view& stringValue) {{ ScarlEntt::TypeReflection::SetValueFromString(this->{property_name}, stringValue); }}
    }};
'''.format(struct_name=struct_name, type=variable[0], type_enum=types[variable[0]], property_name=variable[1]))
            else:
                print("Error processing variable type: " + variable[0])

        f.write('''\
}};

#endif // DEV_CONFIGURATION.

}} // Namespace Scarlett::Component.
'''.format(struct_name=struct_name))

def analyse_components():
    struct_pattern = re.compile(r'\s*struct\s+(\w+)')
    field_pattern = re.compile(r'^\s*(\w[\w\s*&:<>]*?)\s+(\w+)\b')

    braces_level = 0
    for componentLines in get_component_files(ROOT_SEARCH_DIR):
        struct_name         = None
        member_variables    = []
        for line in componentLines:
            if not struct_name:
                match:  Optional[Match[str]] = struct_pattern.search(line)
                if match is not None:
                    struct_name = match.group(1)
                    print("Struct: " + struct_name)
            else:
                match = field_pattern.search(line)
                if line.find('{') != -1:
                    braces_level = braces_level + 1
                if line.find('}') != -1:
                    braces_level = braces_level - 1
                if match and braces_level <= 1:
                    if line.find("(") != -1 and line.find(")") != -1:
                        if line.find("inline") != -1 or ((line.find("(") < line.find("{")) and line.find("{") != -1):
                            continue

                    member_type, member_name = match.groups()
                    member_variables.append((member_type, member_name))
                    print("Field: " + member_name + " -> [" + member_type + "]")

        generate_rtti_for_component(struct_name, member_variables)

analyse_components()