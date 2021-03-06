#pragma once

#include <vector>
#include <random>

template<typename T>
class IChromosome
{
public:
	virtual ~IChromosome(void)  {}

	virtual float Fitness(void) const = 0;
	virtual T& GetValue(void) = 0;
	virtual T const & GetValue(void) const = 0;
};

template<typename Chromosome>
class AGeneticAlgorithm
{
public:
	typedef std::vector<Chromosome> PopulationType;
	typedef std::pair<Chromosome*, Chromosome*> FatherMotherType;

	AGeneticAlgorithm(unsigned int populationSize, unsigned int generationSurviveCount)
	: _chromosomePopulationSize(populationSize)
	, _generationSurvivorCount(generationSurviveCount)
	, _generationNumber(1)
	{
	}

	virtual ~AGeneticAlgorithm(void) { }

	PopulationType const & ChromosomePopulation() const { return this->_chromosomePopulation; }
	PopulationType & ChromosomePopulation() { return this->_chromosomePopulation; }

	void MakeEvolution()
	{
		// elitisme;
		PopulationType newPopulation = this->GetSurvivors();

		// selection/reprod/mutation
		unsigned int i = 0;
		this->PrepareSelection();
		while (newPopulation.size() < this->_chromosomePopulationSize)
		{
			// remarque, on peu faire un BB tout seul ...
			// selection
			FatherMotherType fatherMother = this->Selection();

			// crossover
			PopulationType newChromosome = this->CrossOver(*fatherMother.first, *fatherMother.second);
			// mutation
			this->Mutate(newChromosome);
			newPopulation.insert(newPopulation.end(), newChromosome.begin(), newChromosome.end());
		}

		++_generationNumber;
		_chromosomePopulation = newPopulation;
	}

	virtual Chromosome & GetBestChromosome() = 0;
	unsigned int GenerationNumber() const { return this->_generationNumber; }

	void CreateInitialPopulation()
	{
		for (unsigned int i = 0; i < this->_chromosomePopulationSize; i++)
			_chromosomePopulation.push_back(CreateInitialRandomChromosome());
	}

protected:
	// override this method
	virtual void PrepareSelection() {}
	virtual FatherMotherType Selection() = 0;
	virtual Chromosome CreateInitialRandomChromosome() = 0;
	virtual PopulationType GetSurvivors() = 0;
	virtual void Mutate(PopulationType & population) = 0;
	virtual PopulationType CrossOver(Chromosome const & father, Chromosome const & mother) = 0;



	unsigned int _chromosomePopulationSize;
	unsigned int _generationSurvivorCount;
	unsigned int _generationNumber;

	PopulationType _chromosomePopulation;
};

