#pragma once

#include <string>
#include "bb/Model.h"
#include "bb/FrameBuffer.h"

namespace bb{

template <typename T>
class PassThrough : public Model
{
	protected:
		indices_t m_input_shape;
		indices_t m_output_shape;

	public:
		static std::shared_ptr<PassThrough> Create(){
			return std::make_shared<PassThrough>();
		}

		std::string GetModelName() const override {
			return "PassThrough";
		}

		indices_t SetInputShape(indices_t shape) override {
			this->m_input_shape = shape;
			this->m_output_shape = shape; //identity fn(input and output same shape)
			return shape;
		}

		indices_t GetInputShape() const override {
			return m_input_shape;
		}

		indices_t GetOutputShape() const override {
			return m_output_shape;
		}

		//returns as it is(identity)
		FrameBuffer Forward(FrameBuffer x_buf, bool train = true) override {
			BB_ASSERT(x_buf.GetShape() == this->m_input_shape);
			return x_buf;
		}

		FrameBuffer Backward(FrameBuffer dy_buf) override {
			return dy_buf;
		}

		#ifdef BB_PYBIND11
		static std::shared_ptr<PassThrough> CreatePy(){
			 return Create();
		}
		#endif


};
}
