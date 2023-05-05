#!/bin/bash

# Lê o arquivo linha por linha
while read line; do

  # Verifica se a linha contém a palavra "rainhas"
  if [[ "$line" == *"rainhas"* ]]; then

    # Copia a linha inteira para outro arquivo
    echo "$line" >> $2

    # Inicia a soma dos valores reais
    soma=0
    contador=0


    # Lê as linhas seguintes até encontrar a palavra "rainhas" novamente
    while read line; do

      if [[ "$line" == *"rainhas"* ]]; then

        # Se a linha contém a palavra "rainhas", sai do loop interno
        break
      # Verifica se a linha contém um valor real
    elif [[ "$line" =~ ^[+-]?[0-9]+\.?[0-9]*$ ]]; then

        # Soma o valor real à variável soma
        soma=$(echo "$soma + $line" | bc -l)
        contador=$((contador+1))

      fi

    done

    # Calcula a média dos valores reais e escreve no $2
    media=$(echo "$soma / $contador" | bc -l)
    echo "Média: $media" >> $2

    # Copia a linha com a palavra "rainhas" para o $2
    echo "$line" >> $2

  elif [[ "$line" =~ ^[+-]?[0-9]+\.?[0-9]*$ ]]; then
    # Inicia a soma dos valores reais
    soma=0
    contador=0

    
    # Soma o valor real à variável soma
    soma=$(echo "$soma + $line" | bc -l)
    contador=$((contador+1))

    while read line; do

      if [[ "$line" == *"rainhas"* ]]; then

        # Se a linha contém a palavra "rainhas", sai do loop interno
        break
      # Verifica se a linha contém um valor real
      elif [[ "$line" =~ ^[+-]?[0-9]+\.?[0-9]*$ ]]; then

        # Soma o valor real à variável soma
        soma=$(echo "$soma + $line" | bc -l)
        contador=$((contador+1))

      fi

    done
    # Calcula a média dos valores reais e escreve no $2
    media=$(echo "$soma / $contador" | bc -l)
    echo "Média: $media" >> $2

    # Copia a linha com a palavra "rainhas" para o $2
    echo "$line" >> $2
    
  else
    # Se a linha não contém a palavra "rainhas", continua lendo as linhas seguintes
    continue
  fi

done < $1