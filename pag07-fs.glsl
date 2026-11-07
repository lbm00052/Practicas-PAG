#version 410

out vec4 colorFragmento;

// Uniform con el color difuso del material del modelo
uniform vec3 uColorDifuso;

// 1. Definición del tipo de la subrutina (firma de la función)
subroutine vec4 fCalcularColor();

// 2. Uniform asociado a la subrutina
subroutine uniform fCalcularColor uMetodoColorElegido;

// 3. Implementación 1: Modo Alambre (devuelve color fijo rojo)
subroutine (fCalcularColor)
vec4 colorAlambre(){
    return vec4(1.0, 0.0, 0.0, 1.0);
}

// 3. Implementación 2: Modo Sólido (devuelve el color difuso del material)
subroutine (fCalcularColor)
vec4 colorSolido(){
    return vec4(uColorDifuso, 1.0);
}

void main(){
    // Invoca automáticamente a la subrutina seleccionada desde C++
    colorFragmento = uMetodoColorElegido();
}