#ifndef VIEW_H
#define VIEW_H

class View {
public:
    virtual ~View() = default;
    virtual void draw() = 0;
};

#endif // VIEW_H
