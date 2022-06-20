A Comprehensive Example
=======================

Here, we'll use all that we have learned to construct a small game. Feel free to copy
past this code as-is, compile it, then explore them game before proceeding.

This game has a randomly generate set of rotationally symmetrical polygons with
a varying number of vertices (triangle, squares, pentagons, etc.). In the middle
of the screen is a solid, horizontal line with two perpendicular spikes.

By pressed A or D, the player can rotate the line and spikes left or right. Making the polygon
react in accordance with the physics simulation.

The player is task to get all polygons on one side of the line. If the achieve this, a trigger is
met and the :code:`check_win_condition` method is exeucted.

On top of the spike and line, the player has a circular sprite bearing the telescope logo. This
sprite is a dynamic physics object but can also be moved using the arrow keys. This allows
the player to push along individual polygons, making sorting them onto one side much more achievable.

.. code-block: cpp

    // TODO

