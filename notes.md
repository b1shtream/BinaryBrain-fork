
## understanding binarybrain


### Ques 1.What is the BinaryBrain Buffer (bb.FrameBuffer) and Why Do We Need It?

In BinaryBrain, a bb.FrameBuffer is an intermediate format that stores data (like images or feature maps) in a way that the BinaryBrain computation engine (including logic LUTs, pooling, etc.) can efficiently work with it.

special wrapper like numpy arrays or pytorch tensors that makes it usable inside BinaryBrain’s internal logic-oriented pipeline.

Why Do We Need It?

BinaryBrain is designed to simulate hardware-style computation, often at the bit or frame level. Unlike PyTorch, which works with tensors (mostly floating-point arrays), BinaryBrain supports:

Frame-wise stochastic processing

Quantized data types (like BIT, INT, etc.)

Simulation of logic circuits and stochastic inference

To support that, the input data (from torchvision.datasets.MNIST, which gives PyTorch tensors) must be converted into this special format before being passed into the BinaryBrain model.

```
images, labels = next(iter(loader_test))
x_buf = bb.FrameBuffer.from_numpy(np.array(images).astype(np.float32))

```

    images: A batch of 28×28 tensors (e.g., shape [32, 1, 28, 28])

    x_buf: The same images, converted into a BinaryBrain-readable buffer format

BinaryBrain layers like bb.Convolution2d and bb.Sequential require FrameBuffer objects for input/output.

### Ques 2.  Where Are the Images Stored During FPGA Inference?

- Export the trained network to Verilog

- Generate test images

- Simulate inference using the Verilog design

Where Do Images Go for Inference?

You generate and export test images like this:

```
bb.write_ppm(os.path.join(rtl_sim_path, 'mnist_test_160x120.ppm'), img[:,:120,:160])

```

This creates .ppm image files. These PPM images are the test inputs that you use to simulate the FPGA.

1) Input Preparation (on PC):

    You convert test images from the dataset (e.g., MNIST) into .ppm images
    
    These are stored under something like ../../verilog/mnist/tb_mnist_lut_cnn/mnist_test_*.ppm
    
2) Verilog Testbench Reads Image:

    The Verilog testbench is written to read pixel data from these image files.
    It feeds pixel values (often grayscale 0–255 or normalized) bit-by-bit or pixel-by-pixel into the logic network on FPGA.

3) On Actual FPGA (not simulation):

    If you deploy to real hardware, the image input would come over a communication channel (e.g., UART, SPI, AXI, etc.). The image is loaded into input registers or RAM blocks on the FPGA.
    
     
## Ques 3. What are the components/modules required for creating a deeper network(with 6-10 layers) Example- ResNet(skip connections)?

bb.DifferentiableLut       #Stochastic learnable logic units. Use for each convolution-style layer.

bb.Sequential              #To chain multiple layers (e.g., in blocks).

bb.Convolution2d                #To apply LUT layers as spatial filters (e.g., 3x3 or 4x4).

bb.Xor([main_path, skip_path])           # Residual skip connection using XOR.

bb.PassThrough				 #Identity mapping for skip connections (used with bb.Xor / bb.Or).

bb.StochasticMaxPooling          #Spatial downsampling after convolutional blocks.

bb.Reduce                  #Final reduction to output class probabilities.

bb.LossSoftmaxCrossEntropy      #For training the model using classification loss

bb.MetricsCategoricalAccuracy    #To measure test/train accuracy.

bb.OptimizerAdam                    #Optimizer for training.

bb.FrameBuffer.from_numpy            #Converts PyTorch tensor/numpy image to BinaryBrain buffer format.

bb.save_networks / bb.load_networks       #Save and load models.

bb.make_verilog_lut_cnv_layers         #Export the CNN layers to Verilog.

bb.write_ppm / bb.make_image_tile      #Optional, to visualize or prepare test images

bb.RealToBinary / bb.BinaryToReal        #For inference with BinaryLut-based test network.

bb.BinaryLut.from_sparse_model            #For converting trained DifferentiableLut into binary LUTs.

bb.DType.BIT                          #For bit-level optimized inference networks


## Ques 4. What will be the inputs and outputs of the layer bb.Xor/bb.Or and bb.PassThrough ?

bb.Xor is a bitwise XOR operation layer that takes two input paths (usually with the same shape) and performs a frame-wise XOR operation on them.

Inputs:

    A list of two bb.Layer objects (can be bb.Sequential, bb.Convolution2d, etc.)

    Each input must output a FrameBuffer of the same shape and data type
    
Ex:
```
output = bb.Xor([
    bb.Sequential([conv1, conv2]),   # Main path
    bb.PassThrough()                 # Skip connection (identity)
])

```

Output:

A FrameBuffer that is the XOR of the two inputs
Same shape as the inputs

bb.PassThrough()

bb.PassThrough is a special identity layer. It takes the input from earlier in the graph and passes it through unchanged to be used in skip connections.

Inputs:

    It receives whatever input is provided to the composite layer it's inside, usually via a bb.Sequential or inside bb.Xor.
    
Output:

    Returns the exact same FrameBuffer that it received.

## Ques 5. How inputs and outputs(FrameBuffer objects) of a layer looks like?( Example: lut_layer0_0 = bb.DifferentiableLut([192], batch_norm=False, binarize=False), lut_layer0_1 = bb.DifferentiableLut([32], batch_norm=False, binarize=False)

input= 
output of DifferentiableLut is of shape [192,28,28] 


## Ques 6. How to implement a new layer in C++

1) Implement cpp header file /include/*.h files
- Accept input FrameBuffer<T>
- Return the same buffer unchanged
- support basic methods like SetInputShape, Forward, and optionally Backward

2) Include it in build

3) Write PyBind11 bindings, bind it (similar to DifferentiableLutN, AveragePooling)







