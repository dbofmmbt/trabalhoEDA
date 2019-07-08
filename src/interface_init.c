#include <interface_init.h>
#include <pizza.h>
// #include <movie.h>

InfoModel mainModel = {
    getPizzaFromUser,
    getSamplePizza,
    pizzaSave,
    pizzaRead,
    pizzaSize,
    pizzaFree,
    pizzaGetId,
    pizzaSetId,
    pizzaCategory};

InfoView mainView = {
    pizzaPrint,
    pizzaName,
    pizzaSecIndexName};

// InfoModel mainModel = {
//     getMovieFromUser,
//     getSampleMovie,
//     movieSave,
//     movieRead,
//     movieSize,
//     movieFree,
//     movieGetId,
//     movieSetId,
//     movieGender};

// InfoView mainView = {
//     moviePrint,
//     movieName,
//     movieSecIndexName};
