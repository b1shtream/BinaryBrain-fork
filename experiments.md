## Results

## MNIST 

### 1. StochasticLutSimple (LUT6)

- Training results
	- accuracy: 90.24%, epoch_size: 120, mini_batch_size: 32, lut_binarize: 1, 4 layers
- Synthesis results
	- LEs: 7835 (slack: +89.169ns)
	- clock frequency: 10MHz (constraint- 100ns)
- accuracy : 87%, epoch_size: 8  (7897 LEs)

### 2. StochasticLutCnn (LUT6)

- Training results:
	- accuracy 89.3 %, epoch size: 8, mini_batch_size: 32, lut_binarize: 1

- Synthesis results[ERROR-> generated verilog is incorrect]:
	- LEs: (slack: )
	- clock frequency: 10MHz ( constraint: 100ns )

### 3. StochasticLutSimple (LUT 4):

- Training results (ERROR -> Not able to converge/oscillating accuracies)
	- accuracy:15%,  epoch_size: 120, mini_batch_size: 32, lut_binarize: 1 , 4 layers (608 LEs with slack +94ns )
	- accuracy: 64 %, epoch_size: 10, mini_batch_size: 120, lut_binarize: 1, 7 layers
- Synthesis results:
	- LEs:( slack:  )
	- clock frequenccy: 10 MHz (constraint- 100ns)

### 4. StochasticLutCnn (LUT4)
 - Training results
          - accuracy:
  - Synthesis results:
          - LEs:
          - clock frequenccy: 10 MHz (constraint- 100ns)


## CIFAR10 

### 1. 
