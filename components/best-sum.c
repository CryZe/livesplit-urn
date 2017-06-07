#include "urn-component.h"

typedef struct _UrnBestSum {
    UrnComponent base;
    GtkWidget *container;
    GtkWidget *label;
    GtkWidget *sum_of_bests;
    SumOfBestComponent sum_of_best_component;
} UrnBestSum;
extern UrnComponentOps urn_best_sum_operations;

#define SUM_OF_BEST_SEGMENTS "Sum of best segments"

UrnComponent *urn_component_best_sum_new() {
    UrnBestSum *self;

    self = malloc(sizeof(UrnBestSum));
    if (!self) return NULL;
    self->base.ops = &urn_best_sum_operations;

    self->sum_of_best_component = SumOfBestComponent_new();

    self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_class(self->container, "footer"); /* hack */
    gtk_widget_show(self->container);

    self->label = gtk_label_new(SUM_OF_BEST_SEGMENTS);
    add_class(self->label, "sum-of-bests-self->label");
    gtk_widget_set_halign(self->label, GTK_ALIGN_START);
    gtk_widget_set_hexpand(self->label, TRUE);
    gtk_container_add(GTK_CONTAINER(self->container), self->label);
    gtk_widget_show(self->label);

    self->sum_of_bests = gtk_label_new(NULL);
    add_class(self->sum_of_bests, "sum-of-bests");
    gtk_widget_set_halign(self->sum_of_bests, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(self->container), self->sum_of_bests);
    gtk_widget_show(self->sum_of_bests);

    return (UrnComponent *)self;
}

static void best_sum_delete(UrnComponent *self_) {
    UrnBestSum *self = (UrnBestSum *)self_;
    SumOfBestComponent_drop(self->sum_of_best_component);
    free(self_);
}

static GtkWidget *best_sum_widget(UrnComponent *self) {
    return ((UrnBestSum *)self)->container;
}

static void best_sum_show_game(UrnComponent *self_,
        urn_game *game, urn_timer *timer) {
    UrnBestSum *self = (UrnBestSum *)self_;
    char str[256];
    SumOfBestComponentState state = SumOfBestComponent_state(self->sum_of_best_component, timer->timer);
    gtk_label_set_text(GTK_LABEL(self->label), SumOfBestComponentState_text(state));
    if (game->split_count && timer->sum_of_bests) {
        urn_time_string(str, timer->sum_of_bests);
        gtk_label_set_text(GTK_LABEL(self->sum_of_bests), SumOfBestComponentState_time(state));
    }
    SumOfBestComponentState_drop(state);
}

static void best_sum_clear_game(UrnComponent *self_) {
    UrnBestSum *self = (UrnBestSum *)self_;
    gtk_label_set_text(GTK_LABEL(self->sum_of_bests), "");
}

static void best_sum_draw(UrnComponent *self_, urn_game *game,
        urn_timer *timer) {
    UrnBestSum *self = (UrnBestSum *)self_;
    char str[256];
    remove_class(self->sum_of_bests, "time");
    gtk_label_set_text(GTK_LABEL(self->sum_of_bests), "-");
    SumOfBestComponentState state = SumOfBestComponent_state(self->sum_of_best_component, timer->timer);
    gtk_label_set_text(GTK_LABEL(self->label), SumOfBestComponentState_text(state));
    add_class(self->sum_of_bests, "time");
    gtk_label_set_text(GTK_LABEL(self->sum_of_bests), SumOfBestComponentState_time(state));
    SumOfBestComponentState_drop(state);
}

UrnComponentOps urn_best_sum_operations = {
    .delete = best_sum_delete,
    .widget = best_sum_widget,
    .show_game = best_sum_show_game,
    .clear_game = best_sum_clear_game,
    .draw = best_sum_draw
};
