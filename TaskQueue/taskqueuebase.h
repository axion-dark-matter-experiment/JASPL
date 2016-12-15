#ifndef TASKQUEUEBASE_H
#define TASKQUEUEBASE_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
#include <iostream>
#include <memory>
//OpenCL Headers
#include <CL/cl.h>
#include <CL/cl.hpp>
//Boost Headers
//
//OpenMP Headers
//
//Project specific headers
#include "OpenCLBase/openclbase.h"
#include "TaskItem/taskitem.h"
#include "jTypeTraits/jtypetraits.h"

namespace jaspl {

namespace ocl {

/*!
 * \brief The fundamental structure underlying all OpenCL Functions
 */
class TaskQueueBase : public OpenCLBase {

    friend class TaskItem;

  public:
    /*!
     * \brief Build a new TaskQueueBase and implicitly initialize OpenCL
     * Context, Platform etc.
     *
     * \param device_number
     *
     * Number of the OpenCL device to use
     * as described by cl::Platform::getDevices(CL_DEVICE_TYPE_ALL)
     *
     * Note that this will be the device that every TaskItem
     * will be executed on.
     */
    TaskQueueBase( uint device_number = 0 );
    virtual ~TaskQueueBase() = 0;

    /*!
     * \brief Run all TaskItems that are currently loaded.
     *
     * Items are executed in the order in which they were added with
     * AddTaskItem().
     */
    void Execute();

    /*!
     * \brief Add a new TaskItem into the TaskQueue
     *
     * Items added to the queue need not be unique, repeated
     * TaskItems are allowed.
     *
     * \param item
     *
     * The TaskItem to be inserted into the queue.
     */
    void AddTaskItem( TaskItem &item );

    /*!
     * \brief Remove a TaskItem from the queue (if it exists)
     * \param item
     *
     * The TaskItem to be removed.
     *
     * If item is not currently in the queue this function does
     * nothing.
     * \return true if item was removed, false otherwise.
     */
    bool RemoveTaskItem( TaskItem& item );

    /*!
     * \brief Print all of the current elements in the queue
     * in the order in which they will be executed.
     */
    void PrintContents();

    void ReknewSignal( cl::Buffer& processed_buff , size_t processed_bytes, size_t processed_size );

  protected:
    std::list< TaskItem* > task_queue;

    cl::Buffer signal_input;
    cl::Buffer output;

    size_t signal_size;
    size_t signal_bytes;

    bool on_device = false;

};

}

}

#endif // TASKQUEUEBASE_H
