#pragma once

#include <iostream>
#include <memory>
#include "bb/DataType.h"
#include "bb/FrameBuffer.h"
#include <stdexcept>

namespace bb {
    class Xor {
    public:
        /**
         * @brief Perform pointwise XOR operation on two binary FrameBuffers
         * @param buf1 First input buffer (must be BB_TYPE_BIT)
         * @param buf2 Second input buffer (must be BB_TYPE_BIT)
         * @return New FrameBuffer with XOR results
         * @throws std::runtime_error if shapes don't match or types aren't binary
         */
        static FrameBuffer XorOp(FrameBuffer const& buf1, FrameBuffer const& buf2) {
		//std::cout<<"THIS IS buf1: " << buf1 << std::endl;
		std::cout <<" this is buffer 1 shape: "<< buf1.GetShape()  << "this is buffer 2 shape: " << buf2.GetShape() << std::endl;

		std::cout<<"This IS BUF2: " << buf2 << std::endl;

            if (buf1.GetShape() != buf2.GetShape()) {
                throw std::runtime_error("XorOp: Input buffer shapes must match");
            }
            
            if (buf1.GetType() != BB_TYPE_BIT || buf2.GetType() != BB_TYPE_BIT) {
                throw std::runtime_error("XorOp: Both buffers must be of BIT type");
            }
            
            FrameBuffer result(buf1.GetFrameSize(), buf1.GetShape(), BB_TYPE_BIT);
            
	    auto buf1_ptr = buf1.LockConst<Bit>();
            auto buf2_ptr = buf2.LockConst<Bit>();
            auto res_ptr = result.Lock<Bit>();
            
            const index_t frame_size = buf1.GetFrameSize();

	    std::cout<<"frame_size: " << frame_size << std::endl;
            const index_t node_size = buf1.GetNodeSize();

	    std::cout<< "node_size: "<< node_size <<std::endl;
            
            #pragma omp parallel for
            for (index_t node = 0; node < node_size; ++node) {
                for (index_t frame = 0; frame < frame_size; ++frame) {
                    Bit val1 = buf1_ptr.Get(frame, node);
                    Bit val2 = buf2_ptr.Get(frame, node);
                    res_ptr.Set(frame, node, val1 ^ val2);
                }
            }
	    return result;
	}

	    //Looping over a FrameBuffer(output, input and result buffer)

	    static void printBuffer(FrameBuffer const &buf1, FrameBuffer const &buf2){ 
		    const index_t node_size = buf1.GetNodeSize();
		    const index_t frame_size = buf1.GetFrameSize();

		    FrameBuffer result(buf1.GetFrameSize(), buf1.GetShape(), BB_TYPE_BIT);

		    auto buf1_ptr = buf1.LockConst<Bit>();
		    auto buf2_ptr = buf2.LockConst<Bit>();
		    auto res_ptr = result.Lock<Bit>();

		    for(index_t node=0; node < node_size; node++) {
			    for (index_t frame=0; frame< frame_size; frame++) {
				    Bit val1= buf1_ptr.Get(frame, node);
				    Bit val2= buf2_ptr.Get(frame, node);
				    Bit result = res_ptr.Get(frame, node);
				    std::cout<< "val1: " << val1 << " val2: " << val2 << "result: "<< result << std::endl;	
			    }
		    }
        }
    };

} // namespace bb
