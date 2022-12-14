#pragma once
#include "ufunction.hpp"
#include "upattern.hpp"

#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace utl
{
	/**
	 * @brief Worker class of thread pool
	 * 
	 */
    class poolworker
	{
	private:
		std::atomic<bool> _destroying{false};
		std::atomic<bool> _waiting{true};
		std::thread _thread;
		std::queue<function<void()>> _work_queue;
		std::mutex _queue_mutex;
		std::condition_variable _condition;

		/**
		 * @brief Worker main loop
		 * 
		 */
		void loop()
		{
			while (true)
			{
				function<void()> work;
				{
					std::unique_lock<std::mutex> lock_begin(_queue_mutex);
					_condition.wait(lock_begin, [this]
						{ return !_work_queue.empty() || _destroying; });
					if (_destroying)
						break;
					work = _work_queue.front();
					_waiting = false;
				}

				work();

				{
					std::lock_guard<std::mutex> lock_end(_queue_mutex);
					_work_queue.pop();
					_waiting = true;
					_condition.notify_one();
				}
			}
		}

	public:
		poolworker()
		{
			_thread = std::thread(&poolworker::loop, this);
		}

		~poolworker()
		{
			if (_thread.joinable())
			{
				wait();
				_queue_mutex.lock();
				_destroying = true;
				_condition.notify_one();
				_queue_mutex.unlock();
				_thread.join();
			}
		}

		/**
		 * @brief Pushes lambda function without parameters
		 * 
		 * @tparam _Lambda Lambda function, or class type
		 * @param function Worker function
		 */
		template<class _Lambda>
		void push(_Lambda&& function)
		{
			std::lock_guard<std::mutex> lock(_queue_mutex);
			_work_queue.push(std::forward<_Lambda>(function));
			_condition.notify_one();
		}

		/**
		 * @brief Pushes lambda function with parameters
		 * 
		 * @tparam _Lambda Lambda function, or class type
		 * @tparam _Types Lambda function variadic arguments
		 * @param function Worker function
		 * @param args Variadic arguments
		 */
		template<class _Lambda, class... _Types>
		void push(_Lambda&& function, _Types&& ...args)
		{
			push(
				[function = std::move(function), tup = std::make_tuple(std::move(args)...)]() -> decltype(auto)
				{ 
					return std::apply(function, tup); 
				});
		}

		/**
		 * @brief Wait until all work items have been finished
		 * 
		 */
		void wait()
		{
			std::unique_lock<std::mutex> lock(_queue_mutex);
			_condition.wait(lock, [this]() { return _work_queue.empty(); });
		}

		bool is_free()
		{
			return _waiting;
		}
	};

	/**
	 * @brief Thread pool implementation class
	 * 
	 */
    class threadpool : public singleton<threadpool>
    {
    public:
		threadpool()
		{
			set_worker_count(std::thread::hardware_concurrency());
		}

		threadpool(const size_t& count)
		{
			set_worker_count(count);
		}

		~threadpool()
		{
			wait();
		}

		/**
		 * @brief Pushes functional object as worker task
		 * 
		 * @tparam _Types Variadic arguments
		 * @param args Arguments
		 */
        template<class... _Types>
        void push(_Types&& ...args)
        {
            _workers.at(_current)->push(std::forward<_Types>(args)...);
            if(_current < _total)
                _current = 0;
        }

		/**
		 * @brief Set the worker count
		 * 
		 * @param count Thread number
		 */
		void set_worker_count(const size_t& count)
		{
			_total = count;
			_workers.clear();
			for (auto i = 0; i < _total; i++)
				_workers.emplace_back(std::make_unique<poolworker>());
		}

		/**
		 * @brief Wait while work will be done
		 * 
		 */
		void wait()
		{
			for (auto& worker : _workers)
				worker->wait();
		}
    private:
        std::vector<std::unique_ptr<poolworker>> _workers;
        size_t _total{0}, _current{0};
    };
}