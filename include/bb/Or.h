#pragma once

#include <iostream>
#include <memory>

#include <stdexcept>
#include "bb/FrameBuffer.h"
#include "bb/DataType.h"

namespace bb{

	class Or{
		public:
			static FrameBuffer OrOp(FrameBuffer const& buf1, FrameBuffer const& buf2) {

				if (buf1.GetShape() != buf2.GetShape()) {
					throw std::runtime_error("OrOp: Input Buffer shapes must match");
				}

				if (buf1.GetType() != BB_TYPE_BIT  ||  buf2.GetType() != BB_TYPE_BIT ) {
					throw std::runtime_error("OrOp: Both buffers must be of BIT type");

				}

				FrameBuffer result(buf1.GetFrameSize(), buf1.GetShape(), BB_TYPE_BIT);

				auto buf1_ptr = buf1.LockConst<Bit>();
				auto buf2_ptr = buf2.LockConst<Bit>();
				auto res_ptr = result.Lock<Bit>();

				const index_t frame_size = buf1.GetFrameSize();

				const index_t node_size = buf1.GetNodeSize();

				#pragma omp parallel for
				for(index_t node=0; node< node_size;  ++node){
					for(index_t frame=0; frame< frame_size; ++frame){
							Bit val1= buf1_ptr.Get(frame, node);
							Bit val2 = buf2_ptr.Get(frame, node);
   							res_ptr.Set(frame, node, val1 | val2 );
					}
				}
				return result;

			}
	};
}
