
/**
 * 一写多读序列（Queue Write One Read Many）模块。
 */

#include <akae_object.h>


#if !defined(AKAE_QWORN_H_)
#define AKAE_QWORN_H_
AK_C_HEADER_EXTERN_C_BEGIN


/**
 * 队列用户句柄。
 */
#define AK_QwormReader AK_handle


/**
 * 创建一个队列，并返回该对接的对象句柄。
 */
AK_API AK_Object akae_qworm_create (AK_Object Malloc, AK_size max_reader);

/**
 * 释放队列。
 *
 */
AK_API AK_int akae_qworm_release (AK_Object Object);

/**
 * 向队列请求写入数据。\n
 * 写入过程支持分片数据写入，用户先调用该接口传入对应数据参数发起请求，\n
 * 请求成功以后可以多次调用 @ref akae_qworm_write_chunk() 进行多片数据写入，\n
 * 直至调用 @ref akae_qworm_submit_write() 提交完成一次完整的写入。\n
 * 该接口为线程安全，因此可以在多个线程中调用该接口实现队列一写，\n
 * 但由于数据有序性可能出现紊乱，因此不建议设计上在多个线程调用该接口。
 *
 * @param[IN] Object
 *  对象句柄，在 @ref akae_qworm_create() 调用时获取。
 *
 * @param[IN] seekable
 *  可检索标识，标识该数据为可检索数据，作为队列回收的边界。
 *
 * @param[IN] size
 *  请求写入的数据长度，申请成功以后，在此长度内可以调用 @ref akae_qworm_write_chunk() 进行多次写入，\n
 *  写入总大小不能超过该数值。
 *
 * @param[IN] timestamp
 *  数据时间戳，标识数据的产生时间，原则上是一个一直递增的数字，单位是微秒。\n
 *  如果传入 0 则内部会使用系统时间产生一个时间戳作为该数据时间戳。
 *
 * @param[IN] timeo_us
 *  超时等待，如果传入 -1 表示一直等待，传入 0 表示不进行任何等待，传入大于 0 的数据表示等待对应微秒。\n
 *  后面两种情况在等待超时的时候会返回 @ref AK_ERR_BUSY。
 *
 * @retval AK_OK
 *  请求成功，接下来可以调用 @ref akae_qworm_write_chunk() 往队列写入数据。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  无效对象句柄。
 *
 * @retval AK_ERR_NOT_PERM
 *  操作被禁止，队列此时可能没有可检索数据，而需要用户首先插入一个可检索数据。
 *
 * @retval AK_ERR_BUSY
 *  请求超时，可能上层调用在其他线程中正在操作该接口向队列写入数据。
 *
 * @retval AK_ERR_OUT_OF_MEM
 *  内存不足申请该大小数据并写入队列。
 *
 */
AK_API AK_int akae_qworm_request_write (AK_Object Object, AK_boolean seekable, AK_size size, AK_size64 timestamp,  AK_int timeo_us);

/**
 * 见 @ref akae_qworm_request_write()， 同步请求快速调用。
 */
#define akae_qworm_wait_write(__Object, __seekable, __size, __timestamp) \
	akae_qworm_request_write (__Object, __seekable, __size, __timestamp, -1)

/**
 * 见 @ref akae_qworm_request_write()， 异步请求快速调用。
 */
#define akae_qworm_try_write(__Object, __seekable, __size, __timestamp) \
	akae_qworm_request_write (__Object, __seekable, __size, __timestamp, 0)

/**
 * 写入数据块。
 * 该接口须在 @ref akae_qworm_request_write() 调用成功以后调用。
 *
 * @param Object
 * @param data
 * @param datalen
 * @return
 */
AK_API AK_int akae_qworm_write_chunk (AK_Object Object, AK_voidptr data, AK_size datalen);

/**
 * 提交写入。
 *
 * @param Object
 * @return
 */
AK_API AK_int akae_qworm_submit_write (AK_Object Object);

/**
 * 单次数据写入快速调用。
 */
#define akae_qworm_write(__Object, __seekable, __timestamp,  __data, __datalen) \
	do {\
		if (AK_OK == akae_qworm_wait_write(__Object, __seekable, __datalen, __timestamp)) {\
			akae_qworm_write_chunk (__Object, __data, __datalen);\
			akae_qworm_submit_write (__Object);\
		}\
	} while (0)


/**
 * 从队列中申请一个读用户。\n
 * 上层用户可以通过获取读用户句柄实现从队列中读取数据，实现一写多读的逻辑。\n
 * 获取的用户上限在创建时候 @ref akae_qworm_create() 接口指定，\n
 * 如果获取的用户数超过该上限，会返回 @ref AK_ERR_OUT_OF_RANGE 错误。\n
 * 该接口非线程安全，因此在调用时须注意线程安全。
 *
 * @param[IN] Object
 *  队列对象句柄，通过 @ref akae_qworm_create() 创建。
 *
 * @return
 *  获取成功以后返回读用户句柄（大于0），失败返回对应错误码。
 *
 * @retval AK_OK
 *  申请成功。
 *
 * @retval AK_ERR_INVAL_OBJECT
 *  无效对象句柄。
 *
 * @retval AK_ERR_OUT_OF_RANGE
 *  获取用户数超出了限制，限制由创建时调用 @ref akae_qworm_create() 指定。
 *
 * @retval AK_ERR_BUSY
 *  当前系统使用句柄已经饱和，无法申请系统句柄。
 *
 */
AK_API AK_QwormReader akae_qworm_get_reader (AK_Object Object);

/**
 * 释放用户。\n
 * 当上层调度读行为完成时，需要调用该接口释放用户。\n
 * 与接口 @ref akae_qworm_get_reader() 相对。\n
 * 用户释放以后，队列模块内用户池会回收让下一次读行为提供用户请求。\n
 * 由于读用户有最大申请数限制，在 @ref akae_qworm_create() 中传入。\n
 * 因此当上层应用逻辑完成读行为时须尽快释放读用户。\n
 * 注意的时，读用户时句柄时内部将会对资源进行释放，因此上层调用需要确保句柄在不再使用的条件下进行释放。\n
 *
 * @param[IN] reader
 *  读用户句柄。
 *
 * @return
 *  释放成功返回 AK_OK，失败返回对应的错误码。
 *
 * @retval AK_OK
 *  释放成功。
 *
 * @retval AK_ERR_INVAL_HANDLE
 *  无效对象句柄。
 */
AK_API AK_int akae_qworm_put_reader (AK_QwormReader reader);

/**
 * 读用户定位到最新一个可检索数据。\n
 * 注意，如果队列内最新的可检索数据比当前用户数据要旧，\n
 * 该操作会忽略，仍然会定性为操作成功返回 @ref AK_OK。
 *
 * @param[IN] reader
 *  队列用户句柄。
 *
 * @retval AK_OK
 *  操作成功。
 * @retval AK_ERR_INVAL_HANDLE
 *  操作失败，可能传入了无效的句柄。
 *
 *
 */
AK_API AK_int akae_qworm_reader_seek (AK_QwormReader reader);

/**
 * 队列用户读取一个队列数据，队列每成功读取到一个数据，队列用户内部将会偏移到下一个数据位置。\n
 * 因此读用户一直调用该接口，则会一次得到写入顺序的数据内容，实现先进先出的队列工作原理。\n
 * 如果上层应用希望读取数据以后读用户仍然停留在当前数据位置（一般用于多次读取），可以调用 @ref akae_qworm_reader_peek() 方法。
 *
 * @param[IN] reader
 *  队列用户句柄，通过 @ref akae_qworm_get_reader() 或者 @ref akae_qworm_group() 获取。
 *
 * @param[OUT] sequence
 *  队列数据序列，读取成功时返回，该数值为队列内部自创建起自增的一个整数，每新一个数据想队列写入均会比前一个增加 1，随着写入次数一直递增。
 *
 * @param[OUT] seekable
 *  队列数据可检索标识，读取成功时返回，对应队列写入数据时的对应变量属性。
 *
 * @param[OUT] timestamp
 *  队列数据时间戳，读取成功时返回，对应队列写入数据时的对应变量属性。
 *
 * @param[OUT] data
 *  队列数据内存起始地址，读取成功时返回，对应队列写入数据时的对应变量属性。
 *
 * @return
 *  读取成功时返回 @ref data 返回数据内存有效长度。\n
 *  如果由于读取速度大于写入速度，该接口返回 0 表示在该队列用户会话中读列已经读空。
 *
 * @retval 0
 *  读取失败，此时读列为空，上层可以适当调整读取队列的节奏。
 *
 * @retval AK_ERR_INVAL_HANDLE
 *  读取失败，传入了无效句柄 @ref reader。
 *
 */
AK_API AK_ssize akae_qworm_reader_read (AK_QwormReader reader, AK_size *sequence, AK_boolean *seekable, AK_size64 *timestamp, AK_voidptr *data);

/**
 * 通过 @ref akae_qworm_reader_read() 只读取数据快速调用定义。
 */
#define akae_qworm_reader_read_data_only(__reader, __data) \
		akae_qworm_reader_read (__reader, AK_null, AK_null, AK_null, (__data))

/**
 * 见 @ref akae_qworm_reader_read()，与其差别在于，调用完该接口以后，\n
 * 读用户数据所在位置不会发生向下偏移的逻辑，即上层调用可以通过该接口多次读取同一个队列数据。
 */
AK_API AK_ssize akae_qworm_reader_peek (AK_QwormReader reader, AK_size *sequence, AK_boolean *seekable, AK_size64 *timestamp, AK_voidptr *data);

/**
 * 通过 @ref akae_qworm_reader_peek() 只读取数据快速调用定义。
 */
#define akae_qworm_reader_peek_data_only(__reader, __data) \
		akae_qworm_reader_peek (__reader, AK_null, AK_null, AK_null, (__data))

/**
 * 单组最大队列容量。
 */
#define AK_QWORM_GROUP_MAX_QUEUE (4)


/**
 * 创建组合用户。\n
 * 上层调用可以通过该接口创建一个组合用户，\n
 * 即，看一个用户读取调用可以在多个队列中进行读取。\n
 * 读取的次序策略，根据不同模式下不同，在同步模式下（即参数 @ref sync 为 AK_true），\n
 * 则会等待全部队列中均有数据可读时，比较各个数据时间戳（同步时间戳机制）读出最早一个队列数据，\n
 * 因此，对于存在某些队列写入速度较慢的情况会造成读取延迟。
 * 使用非同步模式不会参考同步时间戳，而优先比较当前有数据的队列。
 *
 * 接口调用成功后，将从各个队列中获取一个读用户，则各个队列的读用户可用数会减少。
 *
 * @param[IN] n
 *  传入队列的个数，如果传入 1 则与 @ref akae_qworm_get_reader() 调用一致。\n
 *  最大传入 @ref AK_QWORM_GROUP_MAX_QUEUE 个队列。
 *
 * @param[IN] Queue
 *  传入一个或者多个队列对象，最大传入 @ref AK_QWORM_GROUP_MAX_QUEUE 个。
 *
 * @return
 *  返回读用户句柄，上层调用可以通过该句柄，通过读用户句柄操作方法操作，如 @ref akae_qworm_reader_read() 等。
 */
AK_API AK_QwormReader akae_qworm_group (AK_boolean sync, AK_size n, AK_Object Oueue, ...);

/**
 * 对应 @ref akae_qworm_group()，释放组用户。
 */
#define akae_qworm_ungroup(__reader) akae_qworm_put_reader(__reader)



/**
 * 获取队列最新写入时间戳。\n
 * 队列时间戳在上层调用 @ref akae_qworm_request_write() 写入接口时更新。\n
 * 通过获取该属性可以定性分析目前队列的一些状态。
 *
 * @return
 *  当前队列时间戳。
 */
AK_API AK_size64 akae_qworm_property_timestamp_written (AK_Object Object);

/**
 * 获取队列数据写入流量，单位 bytes/s。
 */
AK_API AK_size akae_qworm_property_byterate_written (AK_Object Object);

/**
 * 队列生命期内写入的字节数。
 */
AK_API AK_size akae_qworm_property_bytes_written (AK_Object Object);

/**
 * 队列内存使用量，可以定性反映当前媒体内存开销。
 */
AK_API AK_size akae_qworm_property_memory_usage (AK_Object Object);



AK_C_HEADER_EXTERN_C_END
#endif ///< MUTEX_H_
