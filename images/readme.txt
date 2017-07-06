*O padrão do nome das imagens é: label_id (%06d_%08d). O id para cada imagem de um dataset é único. A indexação dos ids e dos labels (rótulos) comecam a partir de 1. Uma imagem com label 0 é uma imagem não rótulada. 

*Os arquivos Features_*_raw.csv guarda uma tabela na qual cada linha esta guardando o raw data de uma imagem. O raw data da imagem foi guardado usando o seguinte padrão

1 - Bases que contem somente imagens em tons de cinza
imagem foi representada como um vetor (canto superior esquerdo da imagem até canto inferior direito da imagem). 

2 - Bases que contem somente imagens em RGB
cada canal da imagem foi represtando como um vetor. A linha da matrix contém a concatenção desses fatores na ordem R-G-B

3 - Bases que contem imagens em RGB e tons de cinza
	3a. se a imagem RGB então ele faz o passo 2
	3b. se a imagem é grey, então ele replica o canal 2 vezes e depois concatena eles de tal form que o vetor fique grey-grey-grey


*Os arquivos path_*.txt contem o caminho para as imagens da base (que ja foram renomeadas conforme o padrão descrito no inicio desse arquivo).

*O script "select_samples" permite criar um subconjunto de alguma base. Para fazer isto basta: 
1. especificar o caminho até o .csv da base,
2. especificar o caminho até o .txt que contêm os camihos (files pathname) de cada imagem da base,
3. definir o nome do arquivo de saida (.csv) contendo o subconjunto
4. definir o nome do arquivo de saida (.txt) contendo o files pathname das amostras do subconjunto
5a. definir o número de amostras para o subconjunto. O script tenta montar um subconjunto balanceado (mesmo número de amostras por classe)
5b. Ou definir o número de amostras para cada classe.
(resaltando, você faz o passo 5a ou o 5b)


Exemplo de uso:
exemple1:
python select_samples COIL100/features_train_raw.csv Path_train.txt meuSubset.csv pathDasImagesDoSubset.csv 10000

exemple2:
(lembrando que a MNIST tem 10 classes)
python select_samples MNIST/features_train_raw.csv Path_train.txt meuSubset.csv pathDasImagesDoSubset.csv 100 100 200 150 400 100 500 120 600 400  