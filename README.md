# indicesLineales
[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/HXTg7mzz)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=11340190)
# Archivos de Índices

Este programa tendrá tres funciones:
1. Descomponer el archivo `superstore.csv` en varios archivos. Esta operación será similar a la operación de *normalización* que se usa en el diseño de bases de datos relacionales. 
2. Crear el índice primario de los archivos. Cada uno de estos índices se almacenarán en un archivo adicional.
3. Recuperar registros usando el índice primario.

## Datos

Seguirás utilizando el archivo de los programas anteriores. A partir de este archivo deberás descomponer los datos en los siguientes archivos:
- `orders.dat` 
- `details.dat`
- `customers.dat`
- `products.dat`

### Descripción de los archivos

**`orders.dat`**
| Campo         | Descripción                              |
|---------------|------------------------------------------|
| Order ID      | Unique Order ID for each Customer.       |
| Order Date    | Order Date of the product.               |
| Ship Date     | Shipping Date of the Product.            |
| Ship Mode     | Shipping Mode specified by the Customer. |
| Customer ID   | Unique ID to identify each Customer.     |


**`details.dat`**
| Campo         | Descripción                              |
|---------------|------------------------------------------|
| Order ID      | Unique Order ID for each Customer.       |
| Product ID    | Unique ID of the Product.                |
| Sales         | Sales of the Product.                    |
| Quantity      | Quantity of the Product.                 |
| Discount      | Discount provided.                       |
| Profit        | Profit/Loss incurred.                    |

**`customers.dat`**
| Campo         | Descripción                              |
|---------------|------------------------------------------|
| Customer ID   | Unique ID to identify each Customer.     |
| Customer Name | Name of the Customer.                    |
| Segment       | The segment where the Customer belongs.  |
| Country       | Country of residence of the Customer.    |
| City          | City of residence of of the Customer.    |
| State         | State of residence of the Customer.      |
| Postal Code   | Postal Code of every Customer.           |
| Region        | Region where the Customer belong.        |

**`products.dat`**
| Campo         | Descripción                              |
|---------------|------------------------------------------|
| Product ID    | Unique ID of the Product.                |
| Category      | Category of the product ordered.         |
| Sub-Category  | Sub-Category of the product ordered.     |
| Product Name  | Name of the Product                      |



> Nota: En teoría el campo `sales` debería estar en `products.csv` pero asumiremos que el precio depende de la venta y que no está fijo por producto.


### Ejemplo de la Descomposición

Si por ejemplo tuvieras los siguientes registros en el archivo original:
```
93,CA-2015-149587,1/31/2015,2/5/2015,Second Class,KB-16315,Karl Braun,Consumer,United States,Minneapolis,Minnesota,55407,Central,OFF-PA-10003177,Office Supplies,Paper,Xerox 1999,12.96,2,0,6.2208
94,CA-2015-149587,1/31/2015,2/5/2015,Second Class,KB-16315,Karl Braun,Consumer,United States,Minneapolis,Minnesota,55407,Central,FUR-FU-10003799,Furniture,Furnishings,"Seth Thomas 13 1/2"" Wall Clock",53.34,3,0,16.5354
95,CA-2015-149587,1/31/2015,2/5/2015,Second Class,KB-16315,Karl Braun,Consumer,United States,Minneapolis,Minnesota,55407,Central,OFF-BI-10002852,Office Supplies,Binders,Ibico Standard Transparent Covers,32.96,2,0,16.1504
```

Entonces la descomposición sería así:

**`orders.dat`**

| Order ID       | Order Date | Ship Date | Ship Mode    | Customer ID |
|----------------|------------|-----------|--------------|-------------|
| CA-2015-149587 | 1/31/2015  | 2/5/2015  | Second Class | KB-16315    |
 

**`details.dat`**
| Order ID       | Product ID      | Sales | Quantity | Discount | Profit  |
|----------------|-----------------|-------|----------|----------|---------|
| CA-2015-149587 | OFF-PA-10003177 | 12.96 | 2        | 0        | 6.2208  |
| CA-2015-149587 | FUR-FU-10003799 | 53.34 | 3        | 0        | 16.5354 |
| CA-2015-149587 | OFF-BI-10002852 | 32.96 | 2        | 0        | 16.1504 |


**`customers.dat`**

| Customer ID | Customer Name | Segment  | Country       | City        | State     | Postal Code | Region  |
|-------------|---------------|----------|---------------|-------------|-----------|-------------|---------|
| KB-16315    | Karl Braun    | Consumer | United States | Minneapolis | Minnesota | 55407       | Central |

**`products.dat`**

| Product ID      | Category        | Sub-Category | Product Name                      |
|-----------------|-----------------|--------------|-----------------------------------|
| OFF-PA-10003177 | Office Supplies | Paper        | Xerox 1999                        |
| FUR-FU-10003799 | Furniture       | Furnishings  | Seth Thomas 13 1/2"" Wall Clock   |
| OFF-BI-10002852 | Office Supplies | Binders      | Ibico Standard Transparent Covers |
 



## La Meta del Programa

### Fase I
La primera fase será la descomposición del archivo. Para este fin deberás recibir el nombre del archivo junto con la bandera `-normalize`.
```
prompt$ make
prompt$ build/indices superstore.csv -normalize
prompt$ ls *.dat
costumers.dat       details.dat     orders.dat      products.dat
```

Al ejecutar el programa con esa bandera, tu programa leerá el archivo `superstore.csv` y creará los archivos de acuerdo a lo mostrado en la sección anterior.

## Fase II
Luego, al tener ya los archivos listos, el siguiente paso será crear los índices lineales para cada uno de los archivos (excepto `details.dat` ¿Sabes por qué?). Esto funcionará así:
```
prompt$ build/indices superstore.csv -create-indices
prompt$ ls *.idx
costumers.idx     orders.idx      products.idx
```

Cada uno de estos archivos de índices lineales contendrá exactamente dos campos: llave y offset. Los campos y registros de estos archivos serán de tamaño fijo. ¿Sabes por qué usamos offset y no RRN?

## Fase III
Una vez construidos los índices tu programa deberá buscar registros usando esas llaves primarias. Esto se hará de la siguiente manera:
```
prompt$ build/indices -search order=CA-2015-149587
ORDER
    ID:          CA-2015-149587
    Date:        1/31/2015
    Shipped:     2/5/2015
    Ship Mode:   Second Class
CUSTOMER
    ID:          KB-16315
    Name:        Karl Braun
    Segment:     Consumer
    Country:     United States
    City:        Minneapolis
    State:       Minnesota
    Postal Code: 55407
    Region:      Central

ITEMS
    OFF-PA-10003177 Office Supplies     Paper       Xerox 1999                          12.96   2   0   6.2208
    FUR-FU-10003799 Furniture           Furnishings Seth Thomas 13 1/2"" Wall Clock     53.34   3   0   16.5354
    OFF-BI-10002852 Office Supplies     Binders     Ibico Standard Transparent Covers   32.96   2   0   16.1504

ORDER SUMMARY
    Items: 3
    Total: 251.86
```

En este caso lo que el programa hará es buscar la orden con ID `CA-2015-149587`. Esto lo hará buscando el índice de las órdenes, una vez encontrado el registro en el índice, procederá al offset indicado ahí y recuperará el registro de la orden. Con ese registro podrás recuperar los registros del cliente, otra vez, usando el índice de clientes. Luego procederás a recuperar los registros de los detalles de la orden. ¿Puedes utilizar algún índice para esto? ¿Cómo buscarás los detalles de la orden? Una vez recuperados estos registros, con el ID de los productos en este detalle, puedes buscar la información de los productos utilizando el índice de productos.

### Otras maneras de usar tu programa

Para buscar los datos de un cliente:
```
prompt$ build/indices -search customer=KB-16315
CUSTOMER
    ID: KB-16315
    Name: Karl Braun
    Segment: Consumer
    Country: United States
    City: Minneapolis
    State: Minnesota
    Postal Code: 55407
    Region: Central
```

Para buscar los datos de un producto
```
prompt$ build/indices -search product=OFF-PA-10003177
PRODUCT
    ID: OFF-PA-10003177
    Category: Office Supplies
    Sub-Category: Paper
    Name: Xerox 1999
```

Por supuesto que si el ID que se usa no se encuentra tu programa deberá reportar un error:
```
prompt$ build/indices -search product=NADA-10003177
Product not found
prompt$ build/indices -search customer=NADA-10003177
Customer not found
prompt$ build/indices -search order=NADA-10003177
Order not found

```

## Algunas Especificaciones
* En ningún momento deberás cargar todo los archivos de datos a memoria. Sin embargo, sí puedes cargar completamente los archivos de índices.
* Tu programa nunca usa `cin`
* Tu programa siempre recibe la información a través de la línea de comando
* Puedes usar estructuras de datos provistas por C++ para almacenar tus índices, por ejemplo `vector` o mejor aún `list` de la STL.
* Los archivos de datos (dat) serán archivos de registros de campos y registros de tamaño fijo. Puedes usar la tabla de tamaños del programa anterior.
* Eres libre de diseñar las clases que consideres conveniente. Es una expectativa que tengas algunas clases para esta tarea. Es una pésima práctica de programación resolver un problema como este colocando todo el código en el `main`. Las consecuencias de hacer algo así se verían reflejadas en la calificación.

## Expectativas del Programa

* El programa debe compilar sin "warnings."
* El programa no deberá tener errores de tiempo de ejecución (no deberá "tronar").
* El programa no deberá tener fugas de memoria (memory leaks). _Aplica cuando haya uso de memoria dinámica_
* El programa deberá seguir buenas prácticas de programación
    * Indentación de código
    * Uso consistente de {}
    * Nombres significativos de identificadores (variables y funciones)
    * Convenciones para los identificadores
        * Variables: camelCase, ejemplo: `rowId`
        * Funciones: CamelCase, ejemplo: `SplitRow`
        * La primera palabra del nombre de una función normalmente debe ser un verbo
        * Datos miembro de clases usan el _ como prefijo, ejemplo: `_root`
    * *NO* usar variables globales
    * Siempre que pases instancias de clases o de structs asegúrate de pasarlos por referencia y `const` cuando sea apropiado.
    * El código debe ser limpio. Remueve el código que hayas comentado, y utiliza lenguaje profesional en tus variables y comentarios.
    * No debes usar `cin` o `cout` en funciones/métodos que no tienen el objetivo de hacer I/O de la consola.

### Documentación de tu Programa

Al principio de cada archivo con código fuente que hayas elaborado deberás colocar comentarios usando el formato que se muestra a continuación:
```
/*
    Title:      Lab 99 – spreadsheet.cpp
    Purpose:    implement the spreadsheet class methods
    Author:     John Wick
    Date:       January 6, 2020
*/
```

Tus funciones deberán estar comentadas también, usando el siguiente formato:
```
/**
* Add Element
* It adds an element to the set. There should not be repeated elements on the set.
* The set should always be sorted. Remember to consider all the possible cases when
* adding to the list:
* - The List is empty
* - The List is not empty and you want to insert before the _head
* @param element the number you want to insert to the set
* @returns true when the element was added to the set, false if the element was not added
* (the element was already on the set)
*/

bool AddElement(double element){
...
}
```

## Evaluación del Programa

* El programa crea correctamente los archivos de datos (`-normalize`), 20
* El programa crea correctamente los archivos de índices (`-create-indices`), 20
* El programa busca correctamente clientes usando índices (`-search customer=ID`), 10
* El programa busca correctamente productos usando índices (`-search product=ID`), 10
* El programa busca correctamente órdenes usando índices (`-search order=ID`), 40
* EXTRA El programa acepta una bandera adicional cuando se usa `-search`, la bandera `-json`. Al usar esta bandera el programa, además de mostrar la salida así como se ha solicitado, escribirá en pantalla el resultado de la búsqueda en JSON. ¡OJO! En el caso de buscar órdenes deberán imprimir el formato correcto..., 40

> Nota: En el caso de no seguir instrucciones podrías no recibir los puntos que reflejan tu esfuerzo. Por ejemplo si no usas las banderas, o no usas parámetros de línea de comando, podrías recibir una calificación de cero.
