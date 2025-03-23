#pragma once

#include "TestLayers.h"
#include <Core/Input/LayerStack.h>

class LayerStackTesting
{
public:
    explicit inline LayerStackTesting(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("LayerStackTesting", "PushingTwoLayersGenerateCorrectId", PushingTwoLayersGenerateCorrectId);
        testRegistry->AddTestCase("LayerStackTesting", "TestingThrowWhenRemovingLayerInIncorrectOrder", TestingThrowWhenRemovingLayerInIncorrectOrder);
        testRegistry->AddTestCase("LayerStackTesting", "TestNoThrowWhenRemovingLayerAfterCreatingOverlay", TestNoThrowWhenRemovingLayerAfterCreatingOverlay);
        testRegistry->AddTestCase("LayerStackTesting", "TestingThrowWhenRemovingLayerThatIsNotValid", TestingThrowWhenRemovingLayerThatIsNotValid);
        testRegistry->AddTestCase("LayerStackTesting", "TestingThrowWhenRemovingOverlayThatIsNotValid", TestingThrowWhenRemovingOverlayThatIsNotValid);
        testRegistry->AddTestCase("LayerStackTesting", "TestingEventHandledOnLayer", TestingEventHandledOnLayer);
        testRegistry->AddTestCase("LayerStackTesting", "TestingEventHandledOnOverlay", TestingEventHandledOnOverlay);
        testRegistry->AddTestCase("LayerStackTesting", "TestingEventHandledOnOverlayBeforeLayer", TestingEventHandledOnOverlayBeforeLayer);
        testRegistry->AddTestCase("LayerStackTesting", "TestingEventHandledOnBothIfOverlayDoesntHandle", TestingEventHandledOnBothIfOverlayDoesntHandle);
    }

    inline static bool PushingTwoLayersGenerateCorrectId()
    {
        bool passed = true;

        Scarlett::LayerStack stack;
        Scarlett::Ref<Scarlett::Layer> layer1 = stack.PushLayer<Scarlett::EmptyLayer>();
        Scarlett::Ref<Scarlett::Layer> layer2 = stack.PushLayer<Scarlett::EmptyLayer>();

        passed &= layer1->GetId() == 0;
        passed &= layer2->GetId() == 1;

        stack.PopLayer(layer2);
        stack.PopLayer(layer1);

        return passed;
    }

    inline static bool TestingThrowWhenRemovingLayerInIncorrectOrder()
    {
        bool passed;

        Scarlett::LayerStack stack;
        Scarlett::Ref<Scarlett::Layer> layer1 = stack.PushLayer<Scarlett::EmptyLayer>();
        Scarlett::Ref<Scarlett::Layer> layer2 = stack.PushLayer<Scarlett::EmptyLayer>();

        try
        {
            stack.PopLayer(layer1);
            stack.PopLayer(layer2);
            passed = false;
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    inline static bool TestNoThrowWhenRemovingLayerAfterCreatingOverlay()
    {
        bool passed;

        Scarlett::LayerStack stack;
        Scarlett::Ref<Scarlett::Layer> layer1 = stack.PushLayer<Scarlett::EmptyLayer>();
        Scarlett::Ref<Scarlett::Overlay> layer2 = stack.PushOverlay<Scarlett::EmptyLayer>();

        try
        {
            stack.PopLayer(layer1);
            stack.PopOverlay(layer2);
            passed = true;
        }
        catch (const std::runtime_error&)
        {
            passed = false;
        }

        return passed;
    }

    inline static bool TestingThrowWhenRemovingLayerThatIsNotValid()
    {
        bool passed;

        Scarlett::LayerStack stack;
        Scarlett::Ref<Scarlett::Layer> layer1 = stack.PushLayer<Scarlett::EmptyLayer>();

        stack.PopLayer(layer1);

        try
        {
            stack.PopLayer(layer1);

            passed = false;
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    inline static bool TestingThrowWhenRemovingOverlayThatIsNotValid()
    {
        bool passed;

        Scarlett::LayerStack stack;
        Scarlett::Ref<Scarlett::Overlay> layer1 = stack.PushOverlay<Scarlett::EmptyLayer>();

        stack.PopOverlay(layer1);

        try
        {
            stack.PopOverlay(layer1);

            passed = false;
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    inline static bool TestingEventHandledOnLayer()
    {
        bool passed = true;

        Scarlett::StaticKeyPressVariableStorage::Reset();

        Scarlett::KeyPressedEvent e{0, 1};

        Scarlett::LayerStack stack;
        auto layer = stack.PushLayer<Scarlett::EmptyLayerForKeyPress>();

        stack.OnEvent(e);

        stack.PopLayer(layer);

        passed &= Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromLayer();
        passed &= !Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromOverlay();

        return passed;
    }

    inline static bool TestingEventHandledOnOverlay()
    {
        bool passed = true;

        Scarlett::StaticKeyPressVariableStorage::Reset();

        Scarlett::KeyPressedEvent e{0, 1};

        Scarlett::LayerStack stack;
        auto overlay = stack.PushOverlay<Scarlett::EmptyOverlayForKeyPress>();

        stack.OnEvent(e);

        stack.PopOverlay(overlay);

        passed &= !Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromLayer();
        passed &= Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromOverlay();

        return passed;
    }

    inline static bool TestingEventHandledOnOverlayBeforeLayer()
    {
        bool passed = true;

        Scarlett::StaticKeyPressVariableStorage::Reset();

        Scarlett::KeyPressedEvent e{0, 1};

        Scarlett::LayerStack stack;
        auto layer = stack.PushLayer<Scarlett::EmptyLayerForKeyPress>();
        auto overlay = stack.PushOverlay<Scarlett::EmptyOverlayForKeyPress>();

        stack.OnEvent(e);

        stack.PopLayer(layer);
        stack.PopOverlay(overlay);

        passed &= !Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromLayer();
        passed &= Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromOverlay();

        return passed;
    }

    inline static bool TestingEventHandledOnBothIfOverlayDoesntHandle()
    {
        bool passed = true;

        Scarlett::StaticKeyPressVariableStorage::Reset();

        Scarlett::KeyPressedEvent e{0, 1};

        Scarlett::LayerStack stack;
        auto layer = stack.PushLayer<Scarlett::EmptyLayerForKeyPress>();
        auto overlay = stack.PushOverlay<Scarlett::EmptyOverlayForKeyPressThatDoesntHandle>();

        stack.OnEvent(e);

        stack.PopLayer(layer);
        stack.PopOverlay(overlay);

        passed &= Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromLayer();
        passed &= Scarlett::StaticKeyPressVariableStorage::HasKeyBeenPressedFromOverlay();

        return passed;
    }
};
