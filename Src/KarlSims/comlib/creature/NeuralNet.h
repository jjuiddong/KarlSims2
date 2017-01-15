/**
 @filename NeuralNet.h
 
  Mat Buckland 2002
*/
#pragma once


namespace evc
{

	//	define neuron struct
	struct sNeuron
	{
		int m_numInputs;
		double m_output;
		vector<double> m_vecWeight;
		sNeuron(int NumInputs);
	};


	//	struct to hold a layer of neurons.
	struct sNeuronLayer
	{
		int m_NumNeurons;
		vector<sNeuron> m_vecNeurons;
		sNeuronLayer(int NumNeurons, int NumInputsPerNeuron);
	};


	//	neural net class
	class cNeuralNet
	{
	public:
		cNeuralNet(const int iNumInputs, const  int iNumOutputs, const  int iNumHidden, const int iNeuronsPerHiddenLayer);
		void CreateNet();
		vector<double> GetWeights() const;
		int GetNumberOfWeights() const;
		void	PutWeights(const vector<double> &weights);
		vector<double> Update(vector<double> &inputs);
		inline double Sigmoid(double activation, double response);


	private:
		int m_numInputs;
		int m_numOutputs;
		int m_numHiddenLayers;
		int m_neuronsPerHiddenLyr;
		vector<sNeuronLayer> m_vecLayers;
		double m_dActivationResponse;
	};
}
