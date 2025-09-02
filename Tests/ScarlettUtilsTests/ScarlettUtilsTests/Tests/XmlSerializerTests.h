#pragma once

#include <string>
#include <fstream>

#include <scarlettTypes/Types.h>
#include <ScarlettUtils/Xml/XmlDocument.h>
#include <ScarlettUtils/Xml/XmlSerializer.h>

class XmlSerializerTests
{
public:
    static constexpr std::string_view TEST_RESOURCE_DIRECTORY{ "E:/Programming/ScarlettProject/Tests/ScarlettUtilsTests/ScarlettUtilsTests/TestResources/" };
    static constexpr std::string_view TEST_RESOURCE_OUTPUT_DIRECTORY{ "E:/Programming/ScarlettProject/Tests/ScarlettUtilsTests/ScarlettUtilsTests/TestResources/Output/" };

    explicit inline XmlSerializerTests(Scarlett::TestRegistry* testRegistry)
    {
        // Serialization Tests.
        testRegistry->AddTestCase("Xml Serialization Tests", "EmptyDocumentDoesNotSerialize", EmptyDocumentDoesNotSerialize);
        testRegistry->AddTestCase("Xml Serialization Tests", "SingleNodeDocumentSerialization", SingleNodeDocumentSerialization);
        testRegistry->AddTestCase("Xml Serialization Tests", "SingleSelfClosingNodeDocumentSerialization", SingleSelfClosingNodeDocumentSerialization);
        testRegistry->AddTestCase("Xml Serialization Tests", "SingleNodeWithinRootNodeDocumentSerialization", SingleNodeWithinRootNodeDocumentSerialization);
        testRegistry->AddTestCase("Xml Serialization Tests", "ComplexDocumentSerialization", ComplexDocumentSerialization);
        testRegistry->AddTestCase("Xml Serialization Tests", "ComplexDocumentWithComplexAttributesSerialization", ComplexDocumentWithComplexAttributesSerialization);
        testRegistry->AddTestCase("Xml Serialization Tests", "SceneTestSerialization", SceneTestSerialization);

        // Deserialization Tests.
        testRegistry->AddTestCase("Xml Deserialization Tests", "EmptyDocumentHasNullptrForRootDeserialization", EmptyDocumentHasNullptrForRootDeserialization);
        testRegistry->AddTestCase("Xml Deserialization Tests", "SingleNodeDocumentDeserialization", SingleNodeDocumentDeserialization);
        testRegistry->AddTestCase("Xml Deserialization Tests", "SingleSelfClosingNodeDocumentDeserialization", SingleSelfClosingNodeDocumentDeserialization);
        testRegistry->AddTestCase("Xml Deserialization Tests", "SingleNodeWithinRootNodeDocumentDeserialization", SingleNodeWithinRootNodeDocumentDeserialization);
        testRegistry->AddTestCase("Xml Deserialization Tests", "ComplexDocumentDeserialization", ComplexDocumentDeserialization);
        testRegistry->AddTestCase("Xml Deserialization Tests", "ComplexDocumentWithComplexAttributesDeserialization", ComplexDocumentWithComplexAttributesDeserialization);
        testRegistry->AddTestCase("Xml Deserialization Tests", "SceneTestDeserialization", SceneTestDeserialization);
    }

    static bool OutputFileHasIdenticalContent(const std::string& filename)
    {
        std::ifstream outputFile(std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename, std::ios::in);
        std::ifstream solutionFile(std::string{ TEST_RESOURCE_DIRECTORY } + filename , std::ios::in);

        if (outputFile.fail() || solutionFile.fail())
        {
            return false;
        }

        std::string line;
        vector<std::string> outputLines;
        vector<std::string> solutionLines;

        while (std::getline(outputFile, line))
        {
            outputLines.push_back(line);
        }

        while (std::getline(solutionFile, line))
        {
            solutionLines.push_back(line);
        }

        try
        {
            for (size_t i{ 0 }; i < outputLines.size(); ++i)
            {
                if (outputLines[i] != solutionLines[i])
                {
                    return false;
                }
            }
        }
        catch (const std::out_of_range&)
        {
            // This means one of the files has more lines than the other, meaning they not the same.
            return false;
        }

        return true;
    }

    // ============================================================================ XML DESERIALIZATION TESTS ============================================================================
    inline static bool EmptyDocumentDoesNotSerialize()
    {
        const ScarlettUtils::XmlDocument emptyDoc;

        return !ScarlettUtils::XmlSerializer::Serialize(emptyDoc, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + "BlankDocument.xml");
    }

    inline static bool SingleNodeDocumentSerialization()
    {
        const std::string filename{ "SingleNodeDocument.xml" };

        ScarlettUtils::XmlElement* node = new ScarlettUtils::XmlElement{ "fooTag","" , "fooValue" };
        const ScarlettUtils::XmlDocument document{ node };

        const bool serialized = ScarlettUtils::XmlSerializer::Serialize(document, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename);

        return serialized && OutputFileHasIdenticalContent(filename);
    }

    inline static bool SingleSelfClosingNodeDocumentSerialization()
    {
        const std::string filename{ "SingleSelfClosingNodeDocument.xml" };

        ScarlettUtils::XmlElement* node = new ScarlettUtils::XmlElement{ "fooTag" };
        node->AddChild("child");

        const ScarlettUtils::XmlDocument document{ node };

        const bool serialized = ScarlettUtils::XmlSerializer::Serialize(document, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename);

        return serialized && OutputFileHasIdenticalContent(filename);
    }

    inline static bool SingleNodeWithinRootNodeDocumentSerialization()
    {
        const std::string filename{ "SingleNodeWithinRootNodeDocument.xml" };

        ScarlettUtils::XmlElement* node = new ScarlettUtils::XmlElement{ "fooTag" };
        node->AddChild("childFoo", "", "childFooValue");

        const ScarlettUtils::XmlDocument document{ node };

        const bool serialized = ScarlettUtils::XmlSerializer::Serialize(document, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename);

        return serialized && OutputFileHasIdenticalContent(filename);
    }

    inline static bool ComplexDocumentSerialization()
    {
        const std::string filename{ "ComplexDocument.xml" };

        ScarlettUtils::XmlElement* rootNode = new ScarlettUtils::XmlElement{ "fooTag" };
        rootNode->AddChild("childFoo", "", "childFooValue");
        ScarlettUtils::XmlElement* child2 = rootNode->AddChild("child2");
        child2->AddChild("grandchild1", "", "value");
        ScarlettUtils::XmlElement* grandchild2 = child2->AddChild("grandchild2");
        grandchild2->AddChild("GreatGrandChild", "", "cool");
        child2->AddChild("grandchild3", "", "someOtherValue");
        rootNode->AddChild("child3");

        const ScarlettUtils::XmlDocument document{ rootNode };

        const bool serialized = ScarlettUtils::XmlSerializer::Serialize(document, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename);

        return serialized && OutputFileHasIdenticalContent(filename);
    }

    inline static bool ComplexDocumentWithComplexAttributesSerialization()
    {
        const std::string filename{ "ComplexDocumentWithComplexAttributes.xml" };

        ScarlettUtils::XmlElement* rootNode = new ScarlettUtils::XmlElement{ "fooTag", R"(attribute="document")" };
        rootNode->AddChild("childFoo", R"(first="first" second="second")", "childFooValue");
        ScarlettUtils::XmlElement* child2 = rootNode->AddChild("child2");
        child2->AddChild("grandchild1", R"(a="a" b="b" c="c")", "value");
        ScarlettUtils::XmlElement* grandchild2 = child2->AddChild("grandchild2");
        grandchild2->AddChild("GreatGrandChild", "", "cool");
        child2->AddChild("grandchild3", "", "someOtherValue");
        rootNode->AddChild("child3", R"(this="child" is="the" coolest="!")");

        const ScarlettUtils::XmlDocument document{ rootNode };

        const bool serialized = ScarlettUtils::XmlSerializer::Serialize(document, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename);

        return serialized && OutputFileHasIdenticalContent(filename);
    }

    inline static bool SceneTestSerialization()
    {
        const std::string filename{ "SceneTest.scarlett_scene" };

        ScarlettUtils::XmlElement* rootNode = new ScarlettUtils::XmlElement{ "Scene" };

        {
            // Entity 1.
            ScarlettUtils::XmlElement* entity1 = rootNode->AddChild("Entity");

            // Entity 1 Components.
            ScarlettUtils::XmlElement* nameComponent = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::Tag")");
            nameComponent->AddChild("name", R"(type="string")", "Entity 0");

            ScarlettUtils::XmlElement* boundingBox = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::BoundingBox")");
            boundingBox->AddChild("localMinimum", R"(type="vec3")", "-0.500000,-0.500000,-0.010000");
            boundingBox->AddChild("localMaximum", R"(type="vec3")", "0.500000,0.500000,0.010000");

            ScarlettUtils::XmlElement* transform = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::Transform")");
            transform->AddChild("translation", R"(type="vec3")", "0.000000,-0.500000,0.000000");
            transform->AddChild("rotation", R"(type="quat")", "0.707107,0.707100,0.000000,0.000000");
            transform->AddChild("scale", R"(type="vec3")", "100.000000,100.000000,0.000000");

            ScarlettUtils::XmlElement* squareSprite = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::SquareSprite")");
            squareSprite->AddChild("material", R"(type="material")", "0");
            squareSprite->AddChild("color", R"(type="vec4")", "1.000000,1.000000,1.000000,0.200000");
        }

        {
            // Entity 2.
            ScarlettUtils::XmlElement* entity2 = rootNode->AddChild("Entity");

            // Entity 2 Components.
            ScarlettUtils::XmlElement* nameComponent = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::Tag")");
            nameComponent->AddChild("name", R"(type="string")", "Entity 1");

            ScarlettUtils::XmlElement* boundingBox = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::BoundingBox")");
            boundingBox->AddChild("localMinimum", R"(type="vec3")", "-0.500000,-0.500000,-0.010000");
            boundingBox->AddChild("localMaximum", R"(type="vec3")", "0.500000,0.500000,0.010000");

            ScarlettUtils::XmlElement* transform = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::Transform")");
            transform->AddChild("translation", R"(type="vec3")", "-0.500000,0.500000,0.500000");
            transform->AddChild("rotation", R"(type="quat")", "1.000000,0.000000,0.000000,0.000000");
            transform->AddChild("scale", R"(type="vec3")", "1.000000,0.020000,0.020000");

            ScarlettUtils::XmlElement* line = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::Line")");
            line->AddChild("start", R"(type="vec3")", "-0.500000,0.500000,0.500000");
            line->AddChild("end", R"(type="vec3")", "0.500000,0.500000,0.500000");
            line->AddChild("color", R"(type="vec4")", "0.160000,0.780000,1.000000,1.000000");
        }

        const ScarlettUtils::XmlDocument document{ rootNode };

        const bool serialized = ScarlettUtils::XmlSerializer::Serialize(document, std::string{ TEST_RESOURCE_OUTPUT_DIRECTORY } + filename);

        return serialized && OutputFileHasIdenticalContent(filename);
    }

    // ============================================================================ XML DESERIALIZATION TESTS ============================================================================
    inline static bool EmptyDocumentHasNullptrForRootDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize( std::string{ TEST_RESOURCE_DIRECTORY } + "BlankDocument.xml");

        return !document.IsValidDocument();
    }

    inline static bool SingleNodeDocumentDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize(std::string{ TEST_RESOURCE_DIRECTORY } + "SingleNodeDocument.xml");

        ScarlettUtils::XmlElement* node = new ScarlettUtils::XmlElement{ "fooTag","" , "fooValue" };
        const ScarlettUtils::XmlDocument solutionDocument{ node };

        return document.IsValidDocument() && solutionDocument == document;
    }

    inline static bool SingleSelfClosingNodeDocumentDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize(std::string{ TEST_RESOURCE_DIRECTORY } + "SingleSelfClosingNodeDocument.xml");

        ScarlettUtils::XmlElement* node = new ScarlettUtils::XmlElement{ "fooTag" };
        node->AddChild("child");

        const ScarlettUtils::XmlDocument solutionDocument{ node };

        return document.IsValidDocument() && solutionDocument == document;
    }

    inline static bool SingleNodeWithinRootNodeDocumentDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize(std::string{ TEST_RESOURCE_DIRECTORY } + "SingleNodeWithinRootNodeDocument.xml");

        ScarlettUtils::XmlElement* node = new ScarlettUtils::XmlElement{ "fooTag" };
        node->AddChild("childFoo", "", "childFooValue");

        const ScarlettUtils::XmlDocument solutionDocument{ node };

        return document.IsValidDocument() && solutionDocument == document;
    }

    inline static bool ComplexDocumentDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize(std::string{ TEST_RESOURCE_DIRECTORY } + "ComplexDocument.xml");

        ScarlettUtils::XmlElement* rootNode = new ScarlettUtils::XmlElement{ "fooTag" };
        rootNode->AddChild("childFoo", "", "childFooValue");
        ScarlettUtils::XmlElement* child2 = rootNode->AddChild("child2");
        child2->AddChild("grandchild1", "", "value");
        ScarlettUtils::XmlElement* grandchild2 = child2->AddChild("grandchild2");
        grandchild2->AddChild("GreatGrandChild", "", "cool");
        child2->AddChild("grandchild3", "", "someOtherValue");
        rootNode->AddChild("child3");

        const ScarlettUtils::XmlDocument solutionDocument{ rootNode };

        return document.IsValidDocument() && solutionDocument == document;
    }

    inline static bool ComplexDocumentWithComplexAttributesDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize(std::string{ TEST_RESOURCE_DIRECTORY } + "ComplexDocumentWithComplexAttributes.xml");

        ScarlettUtils::XmlElement* rootNode = new ScarlettUtils::XmlElement{ "fooTag", R"(attribute="document")" };
        rootNode->AddChild("childFoo", R"(first="first" second="second")", "childFooValue");
        ScarlettUtils::XmlElement* child2 = rootNode->AddChild("child2");
        child2->AddChild("grandchild1", R"(a="a" b="b" c="c")", "value");
        ScarlettUtils::XmlElement* grandchild2 = child2->AddChild("grandchild2");
        grandchild2->AddChild("GreatGrandChild", "", "cool");
        child2->AddChild("grandchild3", "", "someOtherValue");
        rootNode->AddChild("child3", R"(this="child" is="the" coolest="!")");

        const ScarlettUtils::XmlDocument solutionDocument{ rootNode };

        return document.IsValidDocument() && solutionDocument == document;
    }

    inline static bool SceneTestDeserialization()
    {
        const auto document = ScarlettUtils::XmlSerializer::Deserialize(std::string{ TEST_RESOURCE_DIRECTORY } + "SceneTest.scarlett_scene");

        ScarlettUtils::XmlElement* rootNode = new ScarlettUtils::XmlElement{ "Scene" };

        {
            // Entity 1.
            ScarlettUtils::XmlElement* entity1 = rootNode->AddChild("Entity");

            // Entity 1 Components.
            ScarlettUtils::XmlElement* nameComponent = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::Tag")");
            nameComponent->AddChild("name", R"(type="string")", "Entity 0");

            ScarlettUtils::XmlElement* boundingBox = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::BoundingBox")");
            boundingBox->AddChild("localMinimum", R"(type="vec3")", "-0.500000,-0.500000,-0.010000");
            boundingBox->AddChild("localMaximum", R"(type="vec3")", "0.500000,0.500000,0.010000");

            ScarlettUtils::XmlElement* transform = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::Transform")");
            transform->AddChild("translation", R"(type="vec3")", "0.000000,-0.500000,0.000000");
            transform->AddChild("rotation", R"(type="quat")", "0.707107,0.707100,0.000000,0.000000");
            transform->AddChild("scale", R"(type="vec3")", "100.000000,100.000000,0.000000");

            ScarlettUtils::XmlElement* squareSprite = entity1->AddChild("Component", R"(typeId="struct Scarlett::Component::SquareSprite")");
            squareSprite->AddChild("material", R"(type="material")", "0");
            squareSprite->AddChild("color", R"(type="vec4")", "1.000000,1.000000,1.000000,0.200000");
        }

        {
            // Entity 2.
            ScarlettUtils::XmlElement* entity2 = rootNode->AddChild("Entity");

            // Entity 2 Components.
            ScarlettUtils::XmlElement* nameComponent = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::Tag")");
            nameComponent->AddChild("name", R"(type="string")", "Entity 1");

            ScarlettUtils::XmlElement* boundingBox = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::BoundingBox")");
            boundingBox->AddChild("localMinimum", R"(type="vec3")", "-0.500000,-0.500000,-0.010000");
            boundingBox->AddChild("localMaximum", R"(type="vec3")", "0.500000,0.500000,0.010000");

            ScarlettUtils::XmlElement* transform = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::Transform")");
            transform->AddChild("translation", R"(type="vec3")", "-0.500000,0.500000,0.500000");
            transform->AddChild("rotation", R"(type="quat")", "1.000000,0.000000,0.000000,0.000000");
            transform->AddChild("scale", R"(type="vec3")", "1.000000,0.020000,0.020000");

            ScarlettUtils::XmlElement* line = entity2->AddChild("Component", R"(typeId="struct Scarlett::Component::Line")");
            line->AddChild("start", R"(type="vec3")", "-0.500000,0.500000,0.500000");
            line->AddChild("end", R"(type="vec3")", "0.500000,0.500000,0.500000");
            line->AddChild("color", R"(type="vec4")", "0.160000,0.780000,1.000000,1.000000");
        }

        const ScarlettUtils::XmlDocument solutionDocument{ rootNode };

        return document.IsValidDocument() && solutionDocument == document;
    }
};
