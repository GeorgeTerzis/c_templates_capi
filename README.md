# c_templates_capi
it just works
We use headers with some special wording to create mark functions and structures as generics,then we via libclang and we generate macros with the needed arguments to get something close to C++ templates.
I wouldn't recomend anyone using this but it just works eventhough you have to put up with strange circular dependencies sometimes.
