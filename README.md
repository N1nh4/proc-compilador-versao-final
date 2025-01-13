# Especificação da Linguagem Proc
 # Regras Léxicas

- letra ::= `a | b | ... | z | A | B | ... | Z`
  
- digito ::= `0 | 1 | ... | 9`
  
- id ::= `{_} letra { letra | digito | _ }`
  
- intcon ::= `digito { digito }`
  
- realcon ::= `intcon.inticon`
  
- charcon ::= `'ch' | '\n' | '\0'`, onde `ch` denota qualquer caractere imprimível da 
tabela ASCII, como especificado pela função isprint() da lingugem C, 
diferente de `\` (barra invertida) e `'` (apóstrofo).

- stringcon ::= `"{ch}"`,  onde `ch` denota qualquer caractere imprimível da tabela ASCII, 
como especificado pela função isprint() da linguagem C, diferente de  `"` 
(aspas) e do caractere newline.

- idconst ::= indica a ocorrência de uma constante inteira declarada e inicializada 
previamente com `“const int”`

- idproc ::= indica a ocorrência de um nome de procedimento, exceto init
  
- comentário: Comentários iniciam sempre com o sufixo `//` (dupla barra comum) e se 
estendem até a ocorrência do caracter de fim de linha.

# Regras Sintáticas


 
