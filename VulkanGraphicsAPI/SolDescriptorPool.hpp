#pragma once
#include "SolDevice.hpp"

namespace SolEngine::Descriptors
{
    class SolDescriptorPool : private IDisposable
    {
    public:
        class Builder
        {
        public:
            Builder(SolDevice& rSolDevice);

            Builder& AddPoolSize(const VkDescriptorType descriptorType, const uint32_t count);
            Builder& SetPoolFlags(const VkDescriptorPoolCreateFlags flags);
            Builder& SetMaxDescriptorSets(const uint32_t count);

            std::unique_ptr<SolDescriptorPool> Build() const;

        private:
            SolDevice&					      _rSolDevice;
            std::vector<VkDescriptorPoolSize> _poolSizes		{};
            uint32_t						  _maxDescriptorSets{ 1000 };
            VkDescriptorPoolCreateFlags		  _poolFlags		{ 0 };
        };

        SolDescriptorPool(SolDevice& rSolDevice, const uint32_t maxDescriptorSets, const VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~SolDescriptorPool();

        VkDescriptorPool GetDescriptorPool() const { return _descriptorPool; }

        /// <summary>
        /// Allocates a single Descriptor Set from the Descriptor Pool.
        /// </summary>
        bool AllocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& rDescriptor) const;
        void FreeDescriptors(const std::vector<VkDescriptorSet>& descriptors) const;
        void ResetPool();

    private:
        // Inherited via IDisposable
        virtual void Dispose() override;

        SolDevice&       _rSolDevice;
        VkDescriptorPool _descriptorPool;

        friend class SolDescriptorWriter;
    };
}
