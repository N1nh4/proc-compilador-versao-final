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

- prog ::= `{decl_list_var} {decl_def_proc} `

- decl_list_var ::= `[const] tipo decl_var { , decl_var} `

- decl_def_proc ::= `prot idproc ( [[&] tipo { [ ] } { , [&] tipo { [ ] } }]] ) | 
def  ( init | idproc ( [ [&] tipo id1{ [( intcon2 | idconst2 )] } { , 
[&] tipo  id2 { [( intcon2 | idconst2 )] } }] ) ) {decl_list_var} { cmd 
}  endp `

- decl_var ::= `id [ = ( intcon | realcon | charcon ) ] | 
id {[ intcon | idconst ]} [ = { (intcon | realcon | charcon ) {, 
(intcon | realcon | charcon) } } ] `

- tipo ::= `char | int | real | bool `

- cmd ::= `do idproc ( [ expr1 { ,  expr2 }] )   
| 
while (expr)  { cmd }  endw  
| 
| 
var id from expr1 (to | dt) expr2 [ by ( intcon | idconst ) ] { cmd }  
endv 
if ( expr ) { cmd }{ elif  ( expr ) { cmd } } [ else { cmd }] endi | atrib  
  | getout 
 | getint id 
 | getreal id 
 | getchar id 
 | getstr id 
 | putint ( id | intcon ) 
 | putreal ( id | realcon ) 
 | putchar ( id | charcon )  
 | putstr ( id | stringcon ) `

- atrib ::= `id { [ expr ] } = expr  `

- expr ::= `expr_simp [ op_rel  expr_simp ] `

- expr_simp ::= `[+ | – ] termo {(+ | – | ||) termo}`

- termo ::= `fator {(* | / | &&)  fator} `

- fator ::= `id { [ expr ] }  | intcon | realcon | charcon |  ( expr )  | ! fator `

- op_rel ::= `== | != | <= | < | >= | >`

 
