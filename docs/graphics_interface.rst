Internals: Graphics Interface
=============================

Drawing Sprites / Shapes
************************

.. doxygenfunction:: TS_VkBeginDrawPass
.. doxygenfunction:: TS_VkDraw
.. doxygenfunction:: TS_VkEndDrawPass
.. doxygenfunction:: TS_VkAcquireNextImage
.. doxygenfunction:: TS_VkCmdDrawRect
.. doxygenfunction:: TS_VkCmdDrawSprite
.. doxygenfunction:: TS_VkCmdClearColorImage

------------------

Images / Textures
*****************
.. doxygenstruct:: TS_Texture
	:members:

.. doxygenfunction:: TS_VkLoadTexture
.. doxygenfunction:: TS_VkUnloadTexture
.. doxygenfunction:: TS_VmaCreateImage

.. doxygenfunction:: TS_VkCopyBufferToImage
.. doxygenfunction:: TS_VkTransitionImageLayout

------------------

Initializing the State
**********************

To initialize the render state, use:

.. doxygenfunction:: TS_VkInit

which, internally, calls the following functions:

.. doxygenfunction:: TS_VkCreateImageView
.. doxygenfunction:: TS_VmaCreateBuffer
.. doxygenfunction:: TS_VmaCreateImage
.. doxygenfunction:: TS_VkCreateInstance
.. doxygenfunction:: TS_VkCreateSurface
.. doxygenfunction:: TS_VkCreateDevice
.. doxygenfunction:: TS_VmaCreateAllocator

.. doxygenfunction:: TS_VmaCreateBuffers
.. doxygenfunction:: TS_VkCreateSwapchain
.. doxygenfunction:: TS_VkCreateImageViews
.. doxygenfunction:: TS_VkCreateRenderPass
.. doxygenfunction:: TS_VkSetupDepthStencil
.. doxygenfunction:: TS_VkCreateShaderModule
.. doxygenfunction:: TS_VkCreateDescriptorSet
.. doxygenfunction:: TS_VkCreateTrianglePipeline
.. doxygenfunction:: TS_VkCreateFramebuffers
.. doxygenfunction:: TS_VkCreateCommandPool
.. doxygenfunction:: TS_VkAllocateCommandBuffers
.. doxygenfunction:: TS_VkCreateSemaphores
.. doxygenfunction:: TS_VkCreateFences
.. doxygenfunction:: TS_VkPopulateDebugMessengerCreateInfo

------------------

Shutting down the State
***********************

To shut down the render state, use:

.. doxygenfunction:: TS_VkQuit

which internally calls the following functions:

.. doxygenfunction:: TS_VkDestroyFences
.. doxygenfunction:: TS_VkDestroySemaphores
.. doxygenfunction:: TS_VkFreeCommandBuffers
.. doxygenfunction:: TS_VkDestroyCommandPool
.. doxygenfunction:: TS_VkDestroyFramebuffers
.. doxygenfunction:: TS_VkDestroyTrianglePipeline
.. doxygenfunction:: TS_VkDestroyDescriptorSet
.. doxygenfunction:: TS_VkDestroyRenderPass
.. doxygenfunction:: TS_VkTeardownDepthStencil
.. doxygenfunction:: TS_VkDestroyImageViews
.. doxygenfunction:: TS_VkDestroySwapchain
.. doxygenfunction:: TS_VkDestroySemaphores
.. doxygenfunction:: TS_VmaDestroyBuffers
.. doxygenfunction:: TS_VkDestroyTextures
.. doxygenfunction:: TS_VmaDestroyAllocator
.. doxygenfunction:: TS_VkDestroyDevice
.. doxygenfunction:: TS_VkDestroySurface
.. doxygenfunction:: TS_VkDestroyDebugMessenger
.. doxygenfunction:: TS_VkDestroyInstance


------------------

Selecting the Physical Device
*****************************

.. doxygenfunction:: TS_VkSelectPhysicalDevice

------------------

Check Supported Depth Format
****************************

.. doxygenfunction:: TS_VkGetSupportedDepthFormat

------------------

Scratch Buffer
**************

.. doxygenfunction:: TS_VkBeginScratchBuffer
.. doxygenfunction:: TS_VkSubmitScratchBuffer

------------------

Scratch Buffer
**************

.. doxygenfunction:: TS_VkResetCommandBuffer
.. doxygenfunction:: TS_VkBeginCommandBuffer

------------------

Vulkan Queues
*************

.. doxygenfunction:: TS_VkQueueSubmit
.. doxygenfunction:: TS_VkQueuePresent
.. doxygenfunction:: TS_VkSelectQueueFamily

