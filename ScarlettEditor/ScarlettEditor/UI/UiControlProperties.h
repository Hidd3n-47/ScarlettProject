#pragma once

namespace ScarlettEditor
{

struct UiControlProperties
{
    // Property Details.
    std::string propertyName    = "Default";
    std::string propertyId      = "Default";

    // Control Variables.
    float dragSpeed = 0.05f;

    // Text Formatting.
    bool firstLetterUppercase = true;
};

} // Namespace ScarlettEditor.
