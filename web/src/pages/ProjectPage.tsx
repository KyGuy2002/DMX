import { Button } from "@/components/ui/button"
import {
  Card,
  CardAction,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from "@/components/ui/card"

import {
  Item,
  ItemActions,
  ItemContent,
  ItemDescription,
  ItemMedia,
  ItemTitle,
} from "@/components/ui/item"



export default function ProjectPage() {

  return (
    <section className="p-12">
      
      <h1 className="text-4xl font-semibold">Hello, Kylan</h1>
      <p className="text-muted-foreground">
        Welcome to your project dashboard. Here you can manage your projects, view analytics, and collaborate with your team.
      </p>

      <div className="mt-8 grid gap-4 grid-cols-3">

        <Card>
          <CardHeader>
            <CardTitle>Sequences</CardTitle>
            <CardDescription>All animated sequences in this project.</CardDescription>
          </CardHeader>
          <CardContent className="flex flex-col gap-2">
            
            <Item variant="outline">
              <ItemContent>
                <ItemTitle>Motor Idle</ItemTitle>
                <ItemDescription>4:36 - Looping</ItemDescription>
              </ItemContent>
              <ItemActions>
                <Button>Edit</Button>
              </ItemActions>
            </Item>

            <Item variant="outline">
              <ItemContent>
                <ItemTitle>Motor Idle</ItemTitle>
                <ItemDescription>4:36 - Looping</ItemDescription>
              </ItemContent>
              <ItemActions>
                <Button>Edit</Button>
              </ItemActions>
            </Item>

            <Item variant="outline">
              <ItemContent>
                <ItemTitle>Motor Idle</ItemTitle>
                <ItemDescription>4:36 - Looping</ItemDescription>
              </ItemContent>
              <ItemActions>
                <Button>Edit</Button>
              </ItemActions>
            </Item>

          </CardContent>
        </Card>

      </div>

    </section>
  )
}